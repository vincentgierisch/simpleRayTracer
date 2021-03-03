#pragma once

#include <glm/glm.hpp>
#include "models/AABB.hpp"

class NaiveBvhNode {
    public:
        AABB aabb;
        unsigned int leftNode, rightNode;
        unsigned int triangle = UINT32_MAX;
        bool isLeaf() { return triangle != UINT32_MAX; };
};

class BinaryBvhNode {
    public:
        AABB leftBox, rightBox;
        int leftNode, rightNode;
        int triangle = -1;
        bool isLeaf() { return triangle != -1; };
};