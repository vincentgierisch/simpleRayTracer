#include "../../include/models/Scene.hpp"

inline vec3 to_glm(const aiVector3D& v) { return vec3(v.x, v.y, v.z); }

void Scene::load(const std::string path, const std::string &name, const mat4 &trafo) {
    Assimp::Importer importer;

    unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals;  // | aiProcess_FlipUVs  // TODO assimp
    const aiScene* scene_ai = importer.ReadFile(path, flags);

    if (!scene_ai) // handle error
        throw std::runtime_error("ERROR: Failed to load file: " + path + "!");

    std::cout << scene_ai->mNumMaterials << std::endl;

    // load materials
    for (unsigned int i = 0; i < scene_ai->mNumMaterials; i++) {
        Material material;
        aiColor3D col;
        auto mat_ai = scene_ai->mMaterials[i];

        Color colorDiffuse, colorSpecular, colorEmissive;

        // will be roughness or index of refraction
        float tmp;

        if (mat_ai->Get(AI_MATKEY_COLOR_DIFFUSE,  col) == AI_SUCCESS) colorDiffuse = Color(col.r, col.g, col.b, 1.0f);
		if (mat_ai->Get(AI_MATKEY_COLOR_SPECULAR, col) == AI_SUCCESS) colorSpecular = Color(col.r, col.g, col.b, 1.0f);
		if (mat_ai->Get(AI_MATKEY_COLOR_EMISSIVE, col) == AI_SUCCESS) colorEmissive = Color(col.r, col.g, col.b, 1.0f);
		if (mat_ai->Get(AI_MATKEY_SHININESS,      tmp) == AI_SUCCESS) material.roughness = Helper::roughness_from_exponent(tmp);
		if (mat_ai->Get(AI_MATKEY_REFRACTI,       tmp) == AI_SUCCESS) material.ior = tmp;
		if (luma(colorDiffuse) > 1e-4) material.albedo = colorDiffuse;
		else                 material.albedo = colorSpecular;
		material.albedo = glm_to_color(pow(color_to_glm(material.albedo), vec3(2.2f, 2.2f, 2.2f)));
		material.emissive = colorEmissive;

        // ToDo load image

        this->Materials.push_back(material);
    }

    // load mesh
    for (unsigned int i = 0; i < scene_ai->mNumMeshes; i++) {
        const aiMesh* mesh_ai = scene_ai->mMeshes[i];
        uint32_t material_id = scene_ai->mMeshes[i]->mMaterialIndex;

        // ToDo: check if material is emissive
        // load vetices
        for (unsigned int i = 0; i < mesh_ai->mNumVertices; i++) {
            Vertex vertex;
            vertex.pos = to_glm(mesh_ai->mVertices[i]);
            vertex.norm = to_glm(mesh_ai->mNormals[i]);
            if (mesh_ai->HasTextureCoords(0)) {
                vertex.textureCoordinate = vec2(to_glm(mesh_ai->mTextureCoords[0][i]));
            } else {
                vertex.textureCoordinate = vec2(0, 0);
            }
            this->Vertices.push_back(vertex);
        }

        //load triangles
        for (unsigned int i = 0; i < mesh_ai->mNumFaces; i++) {
            const aiFace &face = mesh_ai->mFaces[i];
            // check if face is a triangle
            if (face.mNumIndices == 3) {
                Triangle triangle;
                triangle.a = face.mIndices[0]; 
                triangle.b = face.mIndices[1]; 
                triangle.c = face.mIndices[2]; 
                triangle.material_id = material_id;
                this->Triangles.push_back(triangle);
            } else {
                std::cout << "WARN: Mesh: skipping non-triangle [" << face.mNumIndices << "] face (that the ass imp did not triangulate)!" << std::endl;
            }
        }

    }


}