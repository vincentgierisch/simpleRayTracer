#pragma once

#include "../Color.hpp"
#include <glm/glm.hpp>

using namespace glm;

class Pointlight {
    public:
        const vec3 Position;
        const Color Power;
        Pointlight(vec3 position, Color power): Position(position), Power(power) {};
        inline vec3 getPower() {return 4.0f*float(M_PI)*color_to_glm(this->Power);}
};