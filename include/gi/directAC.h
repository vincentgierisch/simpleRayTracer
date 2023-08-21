#ifndef DIRECTAC_H
#define DIRECTAC_H

#include <utility>

#include "albedoCalculator.h"
#include "../Color.hpp"
#include "../utils/utils.hpp"
#include "../models/TriangleIntersection.hpp"

class DirectAlbedoCalculator : public AlbedoCalculator {
    private:
        std::pair<vec3, float> getRandomVecOnHemisphere(const vec3& norm);
        std::pair<vec3, float> getRandomVecOnHemisphereCosineWeighted(const vec3& norm);
        std::pair<vec3, float> getRandomVecOnHemispherePhongWeighted(const vec3& norm, float n);
        std::pair<vec3, float> getRandomVecOnHemisphereGGXWeighted(const vec3& norm, float n, float a);
    public:
        Color calculateAlbedo(HitPoint& hitpoint, Ray& ray, RayTracer* rt) override;
        ~DirectAlbedoCalculator() override;
};

#endif