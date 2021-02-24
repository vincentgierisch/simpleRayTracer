#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include <png++/png.hpp>
#include <glm/glm.hpp>

#include "Primitives.hpp"
#include "gi/Material.hpp"
#include "gi/Texture.hpp"
#include "../utils/Helper.hpp"
#include "Color.hpp"
#include "../gi/Camera.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/material.h>


class Scene {
    private:
        void getPixelsFromPng(png::image<png::rgb_pixel> image, Color* storage);
        Texture* loadTexture(std::string path);
        Scene(){};
        Scene(Scene const&);
        void operator=(Scene const&);
    public:
        static Scene& getInstance() {
            static Scene instance;
            return instance;
        };
        Camera camera;
        std::vector<Vertex> Vertices;
        std::vector<Triangle> Triangles;
        std::vector<Material> Materials;
        std::vector<Texture*> Textures;
        void load(const std::string path, const std::string &name, const glm::mat4 &trafo = glm::mat4());
        ~Scene();
};