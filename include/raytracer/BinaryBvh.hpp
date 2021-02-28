#pragma once

#include "../RayTracer.hpp"
#include "../BvhNode.hpp"
#include "Helper.hpp"
#include <vector>

class BinaryBvhRayTracer : public RayTracer {
    private:
        std::vector<BinaryBvhNode> _bvhNodes;
        unsigned int _root;
        unsigned int subdivide(std::vector<Triangle> &triangles, std::vector<Vertex> &vertices, unsigned int start, unsigned int end);
    public:
        void init() override;
        bool any_hit(Ray& ray) override;
        TriangleIntersection closest_hit(Ray& ray) override;
};