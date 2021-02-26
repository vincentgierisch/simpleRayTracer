#pragma once

#include <glm/glm.hpp>
#include "models/AABB.hpp"

class BvhNode {
    public:
        AABB aabb;
        unsigned int leftNode, rightNode;
        unsigned int triangle = UINT32_MAX;
        bool isLeaf() { return triangle != UINT32_MAX; };
};