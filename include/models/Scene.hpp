#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "Figures.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/material.h>

class Scene {
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    public:
        void load(const std::string path, const std::string &name, const glm::mat4 &trafo = glm::mat4());
};