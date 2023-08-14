#ifndef ALBEDOCALCULATOR_H
#define ALBEDOCALCULATOR_H

#include "../Color.hpp"
#include "../models/HitPoint.hpp"
#include "../Ray.hpp"
#include "../RayTracer.hpp"


class AlbedoCalculator {
    public:
        virtual Color calculateAlbedo(HitPoint& hitpoint, Ray& ray, RayTracer* rt) = 0;
        virtual ~AlbedoCalculator() = default;
};

#endif