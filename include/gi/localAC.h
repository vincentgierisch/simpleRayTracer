#ifndef LOCALAC_H
#define LOCALAC_H

#include "albedoCalculator.h"
#include "../Color.hpp"

class LocalAlbedoCalculator : public AlbedoCalculator {
    Color calculateAlbedo(HitPoint& hitpoint, Ray& ray, RayTracer* rt) override;
    ~LocalAlbedoCalculator() override;
};

#endif