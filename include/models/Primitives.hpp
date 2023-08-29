#pragma once

#include <glm/glm.hpp>
#include "../Ray.hpp"
#include "TriangleIntersection.hpp"
#include "../Color.hpp"

using namespace glm;

class TriangleIntersection;

class Vertex {
    public:
        vec3 pos;
        vec3 norm;
        vec2 textureCoordinate;
};

class Triangle {
    public:
        uint32_t a, b, c;
        uint32_t material_id;
        TriangleIntersection getIntersection(const Vertex* vertices, const Ray& ray);
};

class LightSource {
    public:
        Triangle* triangle;
        Color power;
        float area;

};