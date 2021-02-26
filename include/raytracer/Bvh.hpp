#pragma once

#include "../RayTracer.hpp"
#include "../BvhNode.hpp"
#include <vector>

class BvhRayTracer : public RayTracer {
    private:
        std::vector<BvhNode> _bvhNodes;
        unsigned int _root;
        unsigned int subdivide(std::vector<Triangle> &triangles, std::vector<Vertex> &vertices, unsigned int start, unsigned int end);
        AABB getBoxFromTriangle(std::vector<Vertex> &vertices, Triangle& triangle);
        vec3 getCenter(std::vector<Vertex> &vertices, const Triangle& triangle);
    public:
        void init() override;
        TriangleIntersection any_hit(Ray& ray) override;
        TriangleIntersection closest_hit(Ray& ray) override;
};