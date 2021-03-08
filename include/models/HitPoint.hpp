#pragma once

#include "../gi/Material.hpp"
#include "../Color.hpp"
#include "Primitives.hpp"
#include "TriangleIntersection.hpp"
#include "Scene.hpp"

class HitPoint {
    private:

    public:
        vec3 x, norm;
        vec2 textureCoordinate;
        Material* material;
        uint32_t triangleReference;
        Color albedo();
        HitPoint(float t, float beta, float gamma, Vertex& a, Vertex& b, Vertex& c, Material* m);
        HitPoint(TriangleIntersection& ti);
};
