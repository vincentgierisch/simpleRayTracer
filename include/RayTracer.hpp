#pragma once

#include "Color.hpp"
#include "models/TriangleIntersection.hpp"
#include "Ray.hpp"
#include "models/Scene.hpp"
#include "models/Primitives.hpp"

class RayTracer {
    public:
        TriangleIntersection any_hit(Ray& ray);
        TriangleIntersection closest_hit(Ray& ray);
};