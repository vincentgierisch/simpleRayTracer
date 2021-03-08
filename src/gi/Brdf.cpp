#include "../../include/gi/Brdf.hpp"

float Brdf::fresnel(float cosThetaI, float n1, float n2) {
    float sinTheta = sqrtf(1.0f - powf(cosThetaI, 2));
    float thetaT = asin((n1/n2)*sinTheta);

    float cosThetaT = cosf(thetaT);
    float Rs = powf((n1*cosThetaI - n2*cosThetaT) / (n1*cosThetaI + n2*cosThetaT), 2);
    float Rp = powf((n1*cosThetaT - n2*cosThetaI) / (n1*cosThetaT + n2*cosThetaI), 2);

    return 0.5f * (Rs + Rp);
}

float LambertianBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    return OneOverPi;
}

float PhongBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    float exp = Helper::exponent_from_roughness(hp.material->roughness);
    vec3 wr = 2.0f * hp.norm * dot(wi, hp.norm) - wi;
    float cos_a = powf(dot(wr, wo), exp);
    return (cos_a * (exp + 1.0f)) / (2.0f * M_PI);
}

float LayeredBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    float R = this->fresnel(dot(hp.norm, wo), 1.0f, hp.material->ior);
    return R*this->Coat->f(hp, wi, wo) + (1 - R) * this->Core->f(hp, wi, wo);
}