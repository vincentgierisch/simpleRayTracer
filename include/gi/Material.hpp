#pragma once

#include "Brdf.hpp"
#include "Texture.hpp"
#include "../Color.hpp"
#include <math.h>

class Material {
    public:
        Color albedo;
        Color emissive;
        Texture* albedo_tex = nullptr;
        float ior = 1.3f;
        float roughness = 0.1f;
        Brdf* brdf = nullptr;
};