#ifndef DIRECTAC_H
#define DIRECTAC_H

#include "albedoCalculator.h"
#include "../Color.hpp"
#include "../utils/utils.hpp"
#include "../models/TriangleIntersection.hpp"

class DirectAlbedoCalculator : public AlbedoCalculator {
    private:
        vec3 getRandomVecOnHemisphere(const vec3& norm);
        vec3 getRandomVecOnHemisphereCosineWeighted(const vec3& norm);
        vec3 getRandomVecOnHemispherePhongWeighted(const vec3& norm, float n);
        vec3 getRandomVecOnHemisphereGGXWeighted(const vec3& norm, float n, float a);
    public:
        Color calculateAlbedo(HitPoint& hitpoint, Ray& ray, RayTracer* rt) override;
        ~DirectAlbedoCalculator() override;
};

#endif