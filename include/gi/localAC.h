#ifndef LOCALAC_H
#define LOCALAC_H

#include "albedoCalculator.h"
#include "../Color.hpp"
#include "../utils/RendererContainer.h"

class LocalAlbedoCalculator : public AlbedoCalculator {
    Color calculateAlbedo(HitPoint& hitpoint, Ray& ray, const RendererContainer& rendererContainer) override;
    ~LocalAlbedoCalculator() override;
};

#endif