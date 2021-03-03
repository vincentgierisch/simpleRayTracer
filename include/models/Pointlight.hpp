#pragma once

#include "../Color.hpp"
#include <glm/glm.hpp>

using namespace glm;

class Pointlight {
    public:
        const vec3 Position;
        const Color Power;
        Pointlight(vec3 position, Color power): Position(position), Power(power) {};
};