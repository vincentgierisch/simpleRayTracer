#ifndef DIRECTAC_H
#define DIRECTAC_H

#include <utility>

#include "albedoCalculator.h"
#include "../Color.hpp"
#include "../utils/utils.hpp"
#include "../models/TriangleIntersection.hpp"
#include "../utils/RendererContainer.h"

class DirectAlbedoCalculator : public AlbedoCalculator {
    private:
        std::tuple<Ray, float, Color> sampleLight(const HitPoint& hitPoint);
        std::pair<vec3, float> getSample(HitPoint& hitPoint, const Ray& ray, ImportanceSamplingType type);
        std::pair<vec3, float> getRandomVecOnHemisphere(const vec3& norm);
        std::pair<vec3, float> getRandomVecOnHemisphereCosineWeighted(const vec3& norm);
        std::pair<vec3, float> getRandomVecOnHemispherePhongWeighted(const vec3& norm, float n);
        std::pair<vec3, float> getRandomVecOnHemisphereGGXWeighted(const vec3& norm, float n, float a);

    public:
        Color calculateAlbedo(HitPoint& hitpoint, Ray& ray, const RendererContainer& rendererContainer) override;
        ~DirectAlbedoCalculator() override;
};

#endif