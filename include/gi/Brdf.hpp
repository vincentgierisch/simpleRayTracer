#pragma once

#include "../models/HitPoint.hpp"
#include "BrdfType.hpp"
#include "../Color.hpp"
#include <glm/glm.hpp>
#include <string>

using namespace glm;

class HitPoint;

class Brdf {
    public:
        const float OneOverPi = (1.0f / M_PI);
        float fresnel (float cosThetaI, float n1, float n2);
        inline float cdot(const vec3 &a, const vec3 &b) {
            float x = a.x*b.x + a.y*b.y + a.z*b.z;
            return x < 0.0f ? 0.0f : x;
        }
        virtual vec3 f(HitPoint& hp, vec3 wi, vec3 wo) = 0;
        virtual ~Brdf() = default;
};

class LambertianBrdf : public Brdf {
    public:
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
}; 


class PhongBrdf : public Brdf {
    public:
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
};

class LayeredBrdf : public Brdf {
    private:
        Brdf* Core;
        Brdf* Coat;
    public:
        LayeredBrdf(Brdf* core, Brdf* coat): Core(core), Coat(coat) {};
        vec3 f(HitPoint& hp, vec3 wi, vec3 wo) override;
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
                    return new PhongBrdf;
                    break;
                }
                case BrdfType::Layered: {
                    Brdf* core = getBrdf(BrdfType::Lambertian);
                    Brdf* coat = getBrdf(BrdfType::Phong);
                    return new LayeredBrdf(core, coat);
                    break;
                }
                default: {
                    return new LambertianBrdf;
                }
            }
        }
};