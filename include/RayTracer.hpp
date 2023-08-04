#pragma once

#include "Color.hpp"
#include "models/TriangleIntersection.hpp"
#include "Ray.hpp"
#include "models/Scene.hpp"
#include "models/Primitives.hpp"

class RayTracer {
    public:
        virtual void init() = 0;
        virtual bool any_hit(Ray& ray) = 0;
        virtual TriangleIntersection closest_hit(Ray& ray) = 0;
        virtual ~RayTracer() = default;
};