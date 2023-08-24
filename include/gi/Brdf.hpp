#pragma once

#include "../models/HitPoint.hpp"
#include "BrdfType.hpp"
#include "../Color.hpp"
#include <glm/glm.hpp>
#include <string>
#include "../utils/utils.hpp"

#include <exception>

using namespace glm;

class HitPoint;

// https://pbr-book.org/3ed-2018/Light_Transport_I_Surface_Reflection/Sampling_Reflection_Functions
class Brdf {
    public:
        const float OneOverPi = (1.0f / M_PI);
        float fresnel (float cosThetaI, float n1, float n2);
        float ggx(float cosThetaH, float roughness);
        float ggxG(float cosThetaH, float roughness);
        vec3 sampleDistribution(const vec2& rndm, float roughness);
        
        virtual vec3 f(HitPoint& hp, vec3 wi, vec3 wo) = 0;
        virtual float getPdf(const HitPoint& hp, const vec3& wi, const vec3& wo) = 0;
        virtual vec3 getSample(HitPoint& hp, const vec3& wo, const vec2& rndm) = 0;
        virtual ~Brdf() = default;
};

class LambertianBrdf : public Brdf {
    public:
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
        float getPdf(const HitPoint& hp, const vec3& wi, const vec3& wo) override;
        vec3 getSample(HitPoint& hp, const vec3& wo, const vec2& rndm) override;
}; 


class PhongBrdf : public Brdf {
    private:
        // is needed because albedo should not be added if it is layered. It is already added in Lambertian
        bool _isCoat = false;
    public:
        PhongBrdf(bool isCoat = false):_isCoat(isCoat){};
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
        float getPdf(const HitPoint& hp, const vec3& wi, const vec3& wo) override;
        vec3 getSample(HitPoint& hp, const vec3& wo, const vec2& rndm) override;

};

class CookTorranceBrdf : public Brdf {
    private:
        bool _isCoat = false;
    public:
        CookTorranceBrdf(bool isCoat = false):_isCoat(isCoat){};
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
        float getPdf(const HitPoint& hp, const vec3& wi, const vec3& wo) override;
        vec3 getSample(HitPoint& hp, const vec3& wo, const vec2& rndm) override;
};

class LayeredBrdf : public Brdf {
    private:
        Brdf* Core;
        Brdf* Coat;
    public:
        LayeredBrdf(Brdf* core, Brdf* coat): Core(core), Coat(coat) {};
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
        float getPdf(const HitPoint& hp, const vec3& wi, const vec3& wo) override;
        vec3 getSample(HitPoint& hp, const vec3& wo, const vec2& rndm) override;
        ~LayeredBrdf() {
            delete Core;
            delete Coat;
        };
};

class BrdfFabric {
    public:
        static inline Brdf* getBrdf (BrdfType type) {
            switch (type)
            {
                case BrdfType::Lambertian: {
                    return new LambertianBrdf;
                    break;
                }
                case BrdfType::Phong: {
                    return new PhongBrdf(false);
                    break;
                }
                case BrdfType::Layered: {
                    Brdf* core = getBrdf(BrdfType::Lambertian);
                    Brdf* coat = new PhongBrdf(true);
                    return new LayeredBrdf(core, coat);
                    break;
                }
                case BrdfType::CookTorrance: {
                    return new CookTorranceBrdf(false);
                    break;
                }
                case BrdfType::LayeredCT: {
                    Brdf* core = getBrdf(BrdfType::Lambertian);
                    Brdf* coat = new CookTorranceBrdf(true);
                    return new LayeredBrdf(core, coat);
                    break;
                }
                default: {
                    return new LambertianBrdf;
                }
            }
            return new LambertianBrdf;
        }
};