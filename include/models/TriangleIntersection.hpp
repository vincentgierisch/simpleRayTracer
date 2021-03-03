#pragma once

#include "Primitives.hpp"

class Triangle;

class TriangleIntersection {
    public:
        float t, beta, gamma;
        Triangle* triangle;
        TriangleIntersection(): t(FLT_MAX) {};
        inline bool isValid() {
            return t != FLT_MAX;
        }
};