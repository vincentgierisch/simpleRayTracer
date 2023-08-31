#include "../../include/gi/localAC.h"

Color LocalAlbedoCalculator::calculateAlbedo(HitPoint& hitpoint, Ray& ray, const RendererContainer& rendererContainer) {
    if (color_to_glm(hitpoint.material->emissive) != vec3(0)) {
        return Color(0.592f, 0.902f, 0.941f);
    }
    std::vector<Color> colors;
    for (Pointlight& pointlight : Scene::getInstance().Pointlights) {
        Color resultColor(0, 0, 0);
        vec3 toPoint = pointlight.Position - hitpoint.x;
        vec3 wi = normalize(toPoint);
        Ray shadowRay(hitpoint.x, wi);
        float distance = sqrtf(dot(toPoint, toPoint));
        shadowRay.setMax(distance);
        if(!rendererContainer.rayTracer->any_hit(shadowRay)) {
            resultColor = pointlight.getPower() * hitpoint.material->brdf->f(hitpoint, -ray.direction, wi) * dot(hitpoint.norm, wi) / (distance * distance);
        }
        colors.push_back(resultColor);
    }
    return getAverageColor(colors);
}

LocalAlbedoCalculator::~LocalAlbedoCalculator() {};