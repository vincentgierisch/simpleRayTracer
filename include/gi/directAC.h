#ifndef DIRECTAC_H
#define DIRECTAC_H

#include "albedoCalculator.h"
#include "../Color.hpp"
#include "../utils/utils.hpp"
#include "../models/TriangleIntersection.hpp"

class DirectAlbedoCalculator : public AlbedoCalculator {
    Color calculateAlbedo(HitPoint& hitpoint, Ray& ray, RayTracer* rt) override;
    ~DirectAlbedoCalculator() override;
};

#endif