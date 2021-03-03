#pragma once

#include "../Color.hpp"
#include <glm/glm.hpp>
#include <string>

using namespace glm;

typedef enum BrdfType {
    Lambertian = 0,
    Phong = 1
}
BrdfType;

class Brdf {
    public:
        const float OneOverPi = 0.3f;
        virtual float f(vec3 x, vec3 wi, vec3 wo) = 0;
};

class LambertianBrdf : public Brdf {
    public:
        inline float f(vec3 x, vec3 wi, vec3 wo) override {
            return OneOverPi;
        }
}; 

class BrdfFabric {
    public:
        static inline Brdf* getBrdf (BrdfType type) {
            switch (type)
            {
                case BrdfType::Lambertian:
                    return new LambertianBrdf;
                    break;
                
                default:
                    return new LambertianBrdf;
            }
        }
};