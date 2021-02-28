#pragma once

#include <glm/glm.hpp>
#include "../models/AABB.hpp"
#include <vector>
#include "../models/Primitives.hpp"

using namespace glm;

namespace raytracer {
    class Helper {
        public:
            static inline AABB getBoxFromTriangle(std::vector<Vertex> &vertices, Triangle& triangle) {
                AABB box;
                box.grow(vertices[triangle.a].pos);
                box.grow(vertices[triangle.b].pos);
                box.grow(vertices[triangle.c].pos);
                return box;
            }

            static inline vec3 getCenter(std::vector<Vertex> &vertices, const Triangle& triangle) {
                return (
                    vertices[triangle.a].pos +
                    vertices[triangle.b].pos +
                    vertices[triangle.c].pos
                ) * 0.333333f;
            }
    };
}