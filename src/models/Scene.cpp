#include "../../include/models/Scene.hpp"

void Scene::load(const std::string path, const std::string &name, const mat4 &trafo) {
    Assimp::Importer importer;

    unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals;  // | aiProcess_FlipUVs  // TODO assimp
    const aiScene* scene_ai = importer.ReadFile(path, flags);

        if (!scene_ai) // handle error
        throw std::runtime_error("ERROR: Failed to load file: " + path + "!");
}