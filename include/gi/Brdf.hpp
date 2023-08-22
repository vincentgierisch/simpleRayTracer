#pragma once

#include "../models/HitPoint.hpp"
#include "BrdfType.hpp"
#include "../Color.hpp"
#include <glm/glm.hpp>
#include <string>
#include "../utils/utils.hpp"

using namespace glm;

class HitPoint;

class Brdf {
    public:
        const float OneOverPi = (1.0f / M_PI);
        float fresnel (float cosThetaI, float n1, float n2);
        float ggx(float cosThetaH, float roughness);
        float ggxG(float cosThetaH, float roughness);
        
        inline float absdot(const vec3 &a, const vec3 &b) {
            float x = a.x*b.x + a.y*b.y + a.z*b.z;
            return x < 0.0f ? -x : x;
        }
        virtual vec3 f(HitPoint& hp, vec3 wi, vec3 wo) = 0;
        virtual float getPdf(HitPoint& hp, vec3 wi, vec3 wo) = 0;
        virtual ~Brdf() = default;
};

class LambertianBrdf : public Brdf {
    public:
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
        float getPdf(HitPoint& hp, vec3 wi, vec3 wo) override;
}; 


class PhongBrdf : public Brdf {
    private:
        // is needed because albedo should not be added if it is layered. It is already added in Lambertian
        bool _isCoat = false;
    public:
        PhongBrdf(bool isCoat = false):_isCoat(isCoat){};
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
        float getPdf(HitPoint& hp, vec3 wi, vec3 wo) override;

};

class CookTorranceBrdf : public Brdf {
    private:
        bool _isCoat = false;
    public:
        CookTorranceBrdf(bool isCoat = false):_isCoat(isCoat){};
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
        float getPdf(HitPoint& hp, vec3 wi, vec3 wo) override;
};

class LayeredBrdf : public Brdf {
    private:
        Brdf* Core;
        Brdf* Coat;
    public:
        LayeredBrdf(Brdf* core, Brdf* coat): Core(core), Coat(coat) {};
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
        float getPdf(HitPoint& hp, vec3 wi, vec3 wo) override;
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