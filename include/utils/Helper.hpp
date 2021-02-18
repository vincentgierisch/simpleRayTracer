#pragma once

#include <math.h>

class Helper {
    public:
        inline static float roughness_from_exponent(float exponent) {
            return sqrtf(2.f / (exponent + 2.f));
        }
        inline static float exponent_from_roughness(float roughness) {
            return 2 / (roughness * roughness) - 2;
        }
};