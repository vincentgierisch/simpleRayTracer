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

// https://www.tobias-franke.eu/log/2014/03/30/notes_on_importance_sampling.html
vec3 Brdf::sampleDistribution(const vec2& rndm, float roughness) {
    const float theta = atanf((roughness * sqrtf(rndm.x)) / sqrtf(1.f - rndm.x));
    if (!std::isfinite(theta)) return vec3(0, 0, 1);
    const float phi = 2.f * float(M_PI) * rndm.y;
    const float sinTheta = sinf(theta);
    return vec3(sinTheta * cosf(phi),
                sinTheta * sinf(phi),
                cosf(theta));
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

vec3 LambertianBrdf::getSample(HitPoint& hp, const vec3& wo, const vec2& rndm) {
    float phi = 2.f*M_PIf*rndm.x;
    float theta = asinf(sqrtf(rndm.y));

    float sinTheta = sinf(theta);
    float cosTheta = cosf(theta);

    vec3 wi = vec3(sinTheta * cosf(phi),
                    sinTheta * sinf(phi),
                    cosTheta);

    return toWorldSpace(wi, hp.norm);
}

// https://boksajak.github.io/files/CrashCourseBRDF.pdf
float LambertianBrdf::getPdf(const HitPoint& hp, const vec3& wi, const vec3& wo) {
    return absdot(hp.norm, wi) * (float)M_1_PI;
}

vec3 PhongBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    if (dot(wi, hp.norm) <= 0) return vec3(0);

	float exponent = exponentFromRoughness(hp.material->roughness);
	vec3 wr = 2.0f*hp.norm*dot(wi,hp.norm)-wi;
    float norm = (exponent + 1.f) / (2.0f * M_PIf32);
    return (float)(powf(cdot(wr, wo), exponent) * norm * cdot(wi,hp.norm)) * (this->_isCoat ? vec3(1) : color_to_glm(hp.albedo()));
}

float PhongBrdf::getPdf(const HitPoint& hp, const vec3& wi, const vec3& wo) {
    float exp = exponentFromRoughness(hp.material->roughness);
    float cosTheta = absdot(hp.norm, wi);
    return powf(cosTheta,exp) * (exp+1.0f) * M_2_PI;
}

vec3 PhongBrdf::getSample(HitPoint& hp, const vec3& wo, const vec2& rndm) {
    float exp = exponentFromRoughness(hp.material->roughness);
    float phi = 2.f*M_PIf*rndm.x;
    float cosTheta = powf(rndm.y, (1.f/(1.f + exp)));
    float sinTheta = sqrtf(1.0f-cosTheta*cosTheta);
    vec3 wi = vec3(
        sinTheta*cosf(phi),
        sinTheta*sinf(phi),
        cosTheta
    );

    vec3 wr = 2.0f*hp.norm*dot(hp.norm,wo) - wo;
	return toWorldSpace(wi, wr);

    // return toWorldSpace(hp.norm, wi);
}

vec3 LayeredBrdf::f(HitPoint& hp, vec3 wi, vec3 wo) {
    float R = this->fresnel(absdot(hp.norm, wo), 1.0f, hp.material->ior);
    return R*this->Coat->f(hp, wi, wo) + (1.0f - R) * this->Core->f(hp, wi, wo);
    // return this->Core->f(hp, wi, wo) + this->Coat->f(hp, wi, wo);
}

float LayeredBrdf::getPdf(const HitPoint& hp, const vec3& wi, const vec3& wo) {
	const float F = fresnel(absdot(hp.norm, wo), 1.0f, hp.material->ior);

    float pdfDiffuse = this->Core->getPdf(hp, wo, wi);

    float pdfSpecular = this->Coat->getPdf(hp, wo, wi);
    return (1.0f-F)*pdfDiffuse + F*pdfSpecular;
}

vec3 LayeredBrdf::getSample(HitPoint& hp, const vec3& wo, const vec2& rndm) {
	const float F = fresnel(absdot(hp.norm, wo), 1.0f, hp.material->ior);
	if (rndm.x >= .5f) {
        // specular
		// vec2 newRand((F-rndm.x)/F, rndm.y);
        vec2 newRand(2.f*rndm.x, rndm.y);
		vec3 wi = this->Coat->getSample(hp, wo, newRand);

        // already in world space
		return wi;
	}
	else {
        // diffuse
		// vec2 newRand((rndm.x-F)/(1.0f-F), rndm.y);
        vec2 newRand(2.f*(rndm.x - .5f), rndm.y);
		vec3 wi = this->Core->getSample(hp, wo, newRand);

        // already in world space
		return wi;
	}
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

float CookTorranceBrdf::getPdf(const HitPoint& hp, const vec3& wi, const vec3& wo) {
    vec3 H = normalize(wo + wi);
    float NdotH = dot(hp.norm, H);
    float HdotV = dot(H, wo);
    float D = ggx(NdotH, hp.material->roughness);

    float ggxPdf = (D * (NdotH<0 ? -NdotH : NdotH)) / (4.f * (HdotV<0 ? -HdotV : HdotV));

    return ggxPdf;
}

vec3 CookTorranceBrdf::getSample(HitPoint& hp, const vec3& wo, const vec2& rndm) {
    vec3 wh = toWorldSpace(sampleDistribution(rndm, hp.material->roughness), hp.norm);
    vec3 wi = 2.0f*wh*dot(wh, wo) - wo;
    return wi;
}