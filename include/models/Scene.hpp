#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>

#include "Primitives.hpp"
#include "gi/Material.hpp"
#include "gi/Texture.hpp"
#include "../utils/Helper.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/material.h>

#include <glm/glm.hpp>

class Scene {
    public:
        std::vector<Vertex> Vertices;
        std::vector<Triangle> Triangles;
        std::vector<Material> Materials;
        std::vector<Texture> Textures;
        void load(const std::string path, const std::string &name, const glm::mat4 &trafo = glm::mat4());
};