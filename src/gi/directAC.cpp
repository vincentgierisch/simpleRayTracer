#include "../../include/gi/directAC.h"

Color DirectAlbedoCalculator::calculateAlbedo(HitPoint& hitpoint, Ray& ray, RayTracer* rt) {
    if (color_to_glm(hitpoint.material->emissive) != vec3(0)) {
        return Color(0.592f, 0.902f, 0.941f);
    }

    Color resultColor(0, 0, 0);

    float z1 = random_float(0, 0.9999f);
    float z2 = random_float(0, 0.9999f);

    float sinTheta = sqrtf(1.f-(z1*z1));
    float phi = 2.f*M_PIf*z2;

    vec3 wi = vec3(sinTheta*cosf(phi),
                    sinTheta*sinf(phi),
                    z1);

    wi = align(wi, hitpoint.norm);
    Ray randomRay(hitpoint.x, wi);
    randomRay.setMax(1000.f);

    TriangleIntersection intersection = rt->closest_hit(randomRay);

    if(intersection.isValid()) {
        HitPoint bouncedHitPoint(intersection);
        vec3 c = color_to_glm(bouncedHitPoint.material->emissive) * hitpoint.material->brdf->f(hitpoint, -ray.direction, randomRay.direction) * cdot(randomRay.direction, hitpoint.norm) * 2.f * M_PIf;
        resultColor = glm_to_color(c);
    }

    return resultColor;
}

DirectAlbedoCalculator::~DirectAlbedoCalculator() {};