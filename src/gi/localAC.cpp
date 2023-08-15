#include "../../include/gi/localAC.h"

Color LocalAlbedoCalculator::calculateAlbedo(HitPoint& hitpoint, Ray& ray, RayTracer* rt) {
    std::vector<Color> colors;
    for (Pointlight& pointlight : Scene::getInstance().Pointlights) {
        Color resultColor(0, 0, 0);
        vec3 toPoint = pointlight.Position - hitpoint.x;
        vec3 wi = normalize(toPoint);
        Ray shadowRay(hitpoint.x, wi);
        float distance = sqrtf(dot(toPoint, toPoint));
        shadowRay.setMax(distance);
        if(!rt->any_hit(shadowRay)) {
            vec3 c = pointlight.getPower() * hitpoint.material->brdf->f(hitpoint, -ray.direction, wi) * dot(hitpoint.norm, wi) / (distance * distance);
            resultColor = glm_to_color(c);
        }
        colors.push_back(resultColor);
    }
    return getAverageColor(colors);
}

LocalAlbedoCalculator::~LocalAlbedoCalculator() {};