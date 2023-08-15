#include "../../include/gi/directAC.h"

Color DirectAlbedoCalculator::calculateAlbedo(HitPoint& hitpoint, Ray& ray, RayTracer* rt) {
    if (color_to_glm(hitpoint.material->emissive) != vec3(0)) {
        return Color(220, 243, 245);
    }

    Color resultColor(0, 0, 0);
    vec3 toPoint = randomVec3OnHemisphere(hitpoint.norm);
    vec3 wi = normalize(toPoint);
    Ray randomRay(hitpoint.x, wi);
    randomRay.setMax(1000.f);

    TriangleIntersection intersection = rt->closest_hit(randomRay);

    if(intersection.isValid()) {
        HitPoint bouncedHitPoint(intersection);
        vec3 c = color_to_glm(bouncedHitPoint.material->emissive) * hitpoint.material->brdf->f(hitpoint, -ray.direction, wi) * dot(wi, hitpoint.norm) * 6.3f;
        resultColor = glm_to_color(c);
    }

    return resultColor;
}

DirectAlbedoCalculator::~DirectAlbedoCalculator() {};