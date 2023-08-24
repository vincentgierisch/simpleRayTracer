#include "../../include/gi/directAC.h"

Color DirectAlbedoCalculator::calculateAlbedo(HitPoint& hitpoint, Ray& ray, RayTracer* rt) {
    if (color_to_glm(hitpoint.material->emissive) != vec3(0)) {
        // return Color(0.592f, 0.902f, 0.941f);
        return Color(1, 1, 1);
    }

    Color resultColor(0, 0, 0);

    float expo = exponentFromRoughness(hitpoint.material->roughness);
    float a = hitpoint.material->ior;

    auto[wi, pdf] = this->getRandomVecOnHemisphere(hitpoint.norm);
    vec2 rndmVec = randomVec2(0, 0.9999f);
    //vec3 wi = hitpoint.material->brdf->getSample(hitpoint, -ray.direction, rndmVec);
    //float pdf = hitpoint.material->brdf->getPdf(hitpoint, wi, -ray.direction);
    Ray randomRay(hitpoint.x, wi);
    randomRay.setMax(1000.f);

    TriangleIntersection intersection = rt->closest_hit(randomRay);

    if(intersection.isValid()) {
        HitPoint bouncedHitPoint(intersection);
        
        Color radiance = bouncedHitPoint.material->emissive;

        // resultColor = radiance * hitpoint.material->brdf->f(hitpoint, -ray.direction, randomRay.direction) * cdot(randomRay.direction, hitpoint.norm) * (1.f/pdf);
        resultColor = radiance * hitpoint.material->brdf->f(hitpoint, -ray.direction, randomRay.direction) * cdot(randomRay.direction, hitpoint.norm) * (1.f/pdf);
    }

    return resultColor;
}

std::pair<vec3, float> DirectAlbedoCalculator::getRandomVecOnHemisphere(const vec3& norm) {
    float z1 = random_float(0, 0.9999f);
    float z2 = random_float(0, 0.9999f);

    float sinTheta = sqrtf(1.f-(z1*z1));
    float phi = 2.f*M_PIf*z2;

    vec3 wi = vec3(sinTheta*cosf(phi),
                    sinTheta*sinf(phi),
                    z1);
    // wi is just aligned to the basic hemisphere at (0,0), we need to align it to the normal
    return {toWorldSpace(wi, norm), 1.f/(2.f*M_PI)};
}

std::pair<vec3, float> DirectAlbedoCalculator::getRandomVecOnHemisphereCosineWeighted(const vec3& norm) {
    float z1 = random_float(0, 0.9999f);
    float z2 = random_float(0, 0.9999f);

    float phi = 2.f*M_PIf*z1;
    float theta = asinf(sqrtf(z2));

    float sinTheta = sinf(theta);
    float cosTheta = cosf(theta);

    vec3 wi = vec3(sinTheta * cosf(phi),
                    sinTheta * sinf(phi),
                    cosTheta);

    return {toWorldSpace(wi, norm), (1.f/M_PI)*cosTheta*sinTheta};
}

std::pair<vec3, float> DirectAlbedoCalculator::getRandomVecOnHemispherePhongWeighted(const vec3& norm, float n) {
    float z1 = random_float(0, 0.9999f);
    float z2 = random_float(0, 0.9999f);

    float phi = 2.f*M_PIf*z1;
    float theta = acosf(powf((1.f - z2), (1.f/(n + 1.f))));

    vec3 wi = vec3(
        sinf(theta)*cosf(phi),
        sinf(theta)*sinf(phi),
        cos(theta)
    );
    
    return {toWorldSpace(wi, norm), (((n+1.f)/(2.f*M_PI))*powf(cosf(theta), n)*sinf(theta))};
}

std::pair<vec3, float> DirectAlbedoCalculator::getRandomVecOnHemisphereGGXWeighted(const vec3& norm, float n, float a) {
    float z1 = random_float(0, 0.9999f);
    float z2 = random_float(0, 0.9999f);

    float phi = 2.f*M_PIf*z1;
    float theta = acosf(sqrtf((1.f - z2)/(z2*((a*a)-1.f)+1.f)));

    vec3 wi = vec3(
        sinf(theta)*cosf(phi),
        sinf(theta)*sinf(phi),
        cos(theta)
    );
    
    return {toWorldSpace(wi, norm), cosf(theta)*sinf(theta)};
}


DirectAlbedoCalculator::~DirectAlbedoCalculator() {};