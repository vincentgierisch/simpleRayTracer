#include "../../include/models/Scene.hpp"

inline vec3 to_glm(const aiVector3D& v) { return vec3(v.x, v.y, v.z); }

void Scene::getPixelsFromPng(png::image<png::rgb_pixel> image, Color* storage){
    unsigned int width = image.get_width();
    unsigned int height = image.get_height();
    #pragma omp parallel for
    for (unsigned int i = 0; i < (width * height); ++i) {
        png::rgb_pixel& pixel = image[i / width][i % width];
        storage[i] = Color(pixel.red, pixel.green, pixel.blue) / 255.0f;
    }
}

Texture* Scene::loadTexture(std::string path){
    png::image<png::rgb_pixel> image(path);
    Texture* texture = new Texture;
    texture->width = image.get_width();
    texture->height = image.get_height();
    texture->name = path;
    texture->Texel = new Color[texture->width * texture->height];
    
    this->getPixelsFromPng(image, texture->Texel);

    return texture;
}

void Scene::load(const std::string path) {
    Assimp::Importer importer;

    unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals;
    const aiScene* scene_ai = importer.ReadFile(path, flags);

    if (!scene_ai) // handle error
        throw std::runtime_error("ERROR: Failed to load file: " + path + "!");

    unsigned materialOffset = this->Materials.size();

    // load materials
    for (unsigned int i = 0; i < scene_ai->mNumMaterials; ++i) {
        Material material;
        aiColor3D col;
        aiString nameAi;
        aiMaterial* mat_ai = scene_ai->mMaterials[i];

        mat_ai->Get(AI_MATKEY_NAME, nameAi);
        material.name = nameAi.C_Str();

        Color colorDiffuse, colorSpecular, colorEmissive;

        // will be roughness or index of refraction
        float tmp;

        if (mat_ai->Get(AI_MATKEY_COLOR_DIFFUSE,  col) == AI_SUCCESS) colorDiffuse = Color(col.r, col.g, col.b);
		if (mat_ai->Get(AI_MATKEY_COLOR_SPECULAR, col) == AI_SUCCESS) colorSpecular = Color(col.r, col.g, col.b);
		if (mat_ai->Get(AI_MATKEY_COLOR_EMISSIVE, col) == AI_SUCCESS) colorEmissive = Color(col.r, col.g, col.b);
		if (mat_ai->Get(AI_MATKEY_SHININESS,      tmp) == AI_SUCCESS) material.roughness = roughness_from_exponent(tmp);
		if (mat_ai->Get(AI_MATKEY_REFRACTI,       tmp) == AI_SUCCESS) material.ior = tmp;
		
        if (luma(colorDiffuse) > 1e-4)
            material.albedo = colorDiffuse;
		else
            material.albedo = colorSpecular;

        material.albedo = glm_to_color(pow(color_to_glm(material.albedo), vec3(2.2f, 2.2f, 2.2f)));
		material.emissive = colorEmissive;

        // Load image
        if (mat_ai->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString ai_Path;
            mat_ai->GetTexture(aiTextureType_DIFFUSE, 0, &ai_Path);
            std::cout << "Texture Path: " << ai_Path.C_Str() << std::endl;
            material.albedo_tex = this->loadTexture("render-data/sibenik/" + std::string(ai_Path.C_Str()));
            Textures.push_back(material.albedo_tex);
        }

        material.brdf = this->Brdfs[this->DefaultBrdfType];
        this->Materials.push_back(material);
    }

    // load mesh
    for (unsigned int i = 0; i < scene_ai->mNumMeshes; ++i) {
        const aiMesh* mesh_ai = scene_ai->mMeshes[i];
        uint32_t material_id = scene_ai->mMeshes[i]->mMaterialIndex + materialOffset;
        uint32_t index_offset = this->Vertices.size();

        // load vetices
        for (unsigned int i = 0; i < mesh_ai->mNumVertices; ++i) {
            Vertex vertex;
            vertex.pos = to_glm(mesh_ai->mVertices[i]);
            vertex.norm = to_glm(mesh_ai->mNormals[i]);
            if (mesh_ai->HasTextureCoords(0)) {
                vertex.textureCoordinate = vec2(to_glm(mesh_ai->mTextureCoords[0][i]));
            } else {
                vertex.textureCoordinate = vec2(0, 0);
            }
            // std::cout << "Vertex: " << vertex.pos.x << "|" << vertex.pos.y << "|" << vertex.pos.z << std::endl;
            this->Vertices.push_back(vertex);
        }

        //load triangles
        for (unsigned int i = 0; i < mesh_ai->mNumFaces; ++i) {
            const aiFace &face = mesh_ai->mFaces[i];
            // check if face is a triangle
            if (face.mNumIndices == 3) {
                Triangle triangle;
                triangle.a = face.mIndices[0] + index_offset; 
                triangle.b = face.mIndices[1] + index_offset; 
                triangle.c = face.mIndices[2] + index_offset; 
                triangle.material_id = material_id;
                this->Triangles.push_back(triangle);

                // std::cout << "Triangle: " << triangle.a << triangle.b << triangle.c << std::endl;
            } else {
                std::cout << "WARN: Mesh: skipping non-triangle [" << face.mNumIndices << "] face" << std::endl;
            }
        }
    }

    std::cout << "Triangle count: " << this->Triangles.size() << std::endl;
}

Scene::~Scene(){
    for (Texture* texture : this->Textures) {
        if (texture != nullptr) {
            delete texture;
        }
    }

    for (std::pair<const BrdfType, Brdf*>& p : this->Brdfs) {
        delete p.second;
    }
}