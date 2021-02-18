#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Vertex {
    vec3 pos;
    vec3 norm;
    vec2 tc;
};

class Triangle {
    uint32_t a, b, c;
    uint32_t material_id;
};