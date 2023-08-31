#include "../../include/gi/directAC.h"

Color DirectAlbedoCalculator::calculateAlbedo(HitPoint& hitpoint, Ray& ray, const RendererContainer& rendererContainer) {
    if (color_to_glm(hitpoint.material->emissive) != vec3(0)) {
        // return Color(0.592f, 0.902f, 0.941f);
        return Color(1, 1, 1);
    }

    Color resultColor(0, 0, 0);

    // light sampling has to be processed different
    if (rendererContainer.samplingType == ImportanceSamplingType::Light) {
        auto[randomRay, pdf, radiance] = this->sampleLight(hitpoint);
        if(!rendererContainer.rayTracer->any_hit(randomRay)) {
            resultColor = radiance * hitpoint.material->brdf->f(hitpoint, -ray.direction, randomRay.direction) * (1.f/pdf);
        }
    } else {
        auto[wi, pdf] = this->getSample(hitpoint, ray, rendererContainer.samplingType);
        Ray randomRay(hitpoint.x, wi);
        randomRay.setMax(1000.f);

        TriangleIntersection intersection = rendererContainer.rayTracer->closest_hit(randomRay);

        if(intersection.isValid()) {
            HitPoint bouncedHitPoint(intersection);
            
            Color radiance = bouncedHitPoint.material->emissive;
            resultColor = radiance * hitpoint.material->brdf->f(hitpoint, -ray.direction, randomRay.direction) * cdot(randomRay.direction, hitpoint.norm) * (1.f/pdf);
        }
    }

    return resultColor;
}

std::pair<vec3, float> DirectAlbedoCalculator::getSample(HitPoint& hitPoint, const Ray& ray, ImportanceSamplingType type) {
    switch (type)
    {
        case ImportanceSamplingType::Cosine:
        {
            return this->getRandomVecOnHemisphereCosineWeighted(hitPoint.norm);
            break;
        }
        case ImportanceSamplingType::Brdf:
        {
            vec2 rndmVec = randomVec2(0, 0.9999f);
            vec3 wi = hitPoint.material->brdf->getSample(hitPoint, -ray.direction, rndmVec);
            float pdf = hitPoint.material->brdf->getPdf(hitPoint, wi, -ray.direction);
            return {wi, pdf};
            break;
        }
        case ImportanceSamplingType::Uniform:
        {
            return this->getRandomVecOnHemisphere(hitPoint.norm);
            break;
        }
        default:
        {
            return{vec3(0), 1.f};
            break;
        }
    }
}

std::tuple<Ray, float, Color> DirectAlbedoCalculator::sampleLight(const HitPoint& hitPoint) {
    Scene& scene = Scene::getInstance();
    // get random light
    int randId = rand()%scene.LightSources.size();
    LightSource* ls = &scene.LightSources[randId];
    auto[toLight, LightNorm] = randomPointOnTriangle(scene.Vertices[ls->triangle->a], scene.Vertices[ls->triangle->b], scene.Vertices[ls->triangle->c]);
    vec3 wi = toLight-hitPoint.x;
    Ray ray(hitPoint.x, normalize(wi));
    
    float tmax = length(wi);
	wi /= tmax;
	ray.setMax(tmax);

    float cosTheta = dot(LightNorm,-wi);
	if (cosTheta <= 0.0f) return {ray, 0.0f, Color(0)};
	float pdf = tmax*tmax/(cosTheta * ls->area);
    return {ray, pdf, ls->power};
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
    return {toWorldSpace(wi, norm), 1.f/(2.f*M_PIf)};
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