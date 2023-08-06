#include "../../include/gi/Brdf.hpp"

float Brdf::fresnel(float cosThetaI, float ior_medium, float ior_material) {
    const float n1 = cosThetaI < 0.0f ? ior_material : ior_medium;
    const float n2 = cosThetaI < 0.0f ? ior_medium : ior_material;
    cosThetaI = glm::clamp(glm::abs(cosThetaI), 0.0f, 1.0f);

    float sinTheta = sqrtf(1.0f - powf(cosThetaI, 2));
    if ((n1/n2)*sinTheta >= 1.0f) return 1.0f;
    float thetaT = asinf((n1/n2)*sinTheta);

    float cosThetaT = cosf(thetaT);
    float Rs = powf((n1*cosThetaI - n2*cosThetaT) / (n1*cosThetaI + n2*cosThetaT), 2);
    float Rp = powf((n1*cosThetaT - n2*cosThetaI) / (n1*cosThetaT + n2*cosThetaI), 2);

    return 0.5f * (Rs + Rp);
}

vec3 LambertianBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    return OneOverPi * color_to_glm(hp.albedo());
}

vec3 PhongBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    if (dot(wi, hp.norm) <= 0) return vec3(0);
    /*
    float exp = Helper::exponent_from_roughness(hp.material->roughness);
    vec3 wr = 2.0f * hp.norm * dot(wi, hp.norm) - wi;
    float cos_a = powf(dot(wr, wo), exp);
    return ((cos_a * (exp + 1.0f)) / (2.0f * M_PIf32)) * color_to_glm(hp.albedo());
    */
	float exponent = Helper::exponent_from_roughness(hp.material->roughness);
	vec3 wr = 2.0f*hp.norm*dot(wi,hp.norm)-wi;
	// const float norm_f = (exponent + 2.0f) * (1.0f / (2.0f * M_PI));
	// return color_to_glm(hp.albedo()) * powf(cos_theta, exponent) * norm_f * this->cdot(wi,hp.norm);
    float norm = (exponent + 2.0f) / (2.0f * M_PIf32);
    return (float)(powf(cdot(wr, wo), exponent) * norm * cdot(wi,hp.norm)) * (this->_isCoat ? vec3(1) : color_to_glm(hp.albedo()));
}

vec3 LayeredBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    float R = this->fresnel(this->absdot(hp.norm, wo), 1.0f, hp.material->ior);
    return R*this->Coat->f(hp, wi, wo) + (1.0f - R) * this->Core->f(hp, wi, wo);
    // return this->Core->f(hp, wi, wo) + this->Coat->f(hp, wi, wo);
}

vec3 CookTorranceBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    return color_to_glm(hp.albedo());
}