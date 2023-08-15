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

float Brdf::ggx(float cosThetaH, float roughness) {
    if (cosThetaH <= 0) return 0.f;

    float cosSThetaH = cosThetaH * cosThetaH;
    if (cosSThetaH <= 0) return 0.f;
    float sRoughness = roughness * roughness;

    return sRoughness / (float(M_PI) * (1.0f + (sRoughness - 1.0f) * cosSThetaH)*(1.0f + (sRoughness - 1.0f) * cosSThetaH));
}

float Brdf::ggxG(float cosThetaH, float roughness) {

    if (cosThetaH <= 0) return 0.f;
    float sRoughness = roughness * roughness;
    float cosSThetaH = cosThetaH * cosThetaH;
    float lambda = 0.5f * (sqrtf(1.0f + (sRoughness/cosSThetaH)-sRoughness) - 1.0f);
    return 1 / (1 + lambda);
}

vec3 LambertianBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    return OneOverPi * color_to_glm(hp.albedo());
}

vec3 PhongBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    if (dot(wi, hp.norm) <= 0) return vec3(0);

	float exponent = exponent_from_roughness(hp.material->roughness);
	vec3 wr = 2.0f*hp.norm*dot(wi,hp.norm)-wi;
    float norm = (exponent + 2.0f) / (2.0f * M_PIf32);
    return (float)(powf(cdot(wr, wo), exponent) * norm * cdot(wi,hp.norm)) * (this->_isCoat ? vec3(1) : color_to_glm(hp.albedo()));
}

vec3 LayeredBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    float R = this->fresnel(this->absdot(hp.norm, wo), 1.0f, hp.material->ior);
    return R*this->Coat->f(hp, wi, wo) + (1.0f - R) * this->Core->f(hp, wi, wo);
    // return this->Core->f(hp, wi, wo) + this->Coat->f(hp, wi, wo);
}

vec3 CookTorranceBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    // if (dot(wi, hp.norm) <= 0) return vec3(0);
    
    float NDotWi = dot(hp.norm, wi);
    float NDotWo = dot(hp.norm, wo);
    // bounding check
    if (NDotWo == 0.f || NDotWi == 0.f) return vec3(0);
    // https://boksajak.github.io/files/CrashCourseBRDF.pdf
    vec3 H = normalize(wo+wi);
    float NDotH = dot(hp.norm, H);
    float roughness = hp.material->roughness;
    float F = this->fresnel(dot(H, wi), 1.0f, hp.material->ior);
    float D = ggx(NDotH, roughness);
    float G = ggxG(NDotWi, roughness) * ggxG(NDotWo, roughness);
    float f = (F*D*G) / (4.0f * abs(NDotWi) * abs(NDotWo));
    
    return this->_isCoat ? vec3(f) : f * 15.0f * color_to_glm(hp.albedo());
}