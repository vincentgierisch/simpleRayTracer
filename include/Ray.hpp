#ifndef RAY_HPP
#define RAY_HPP

#include <glm/glm.hpp>

using namespace glm;

class Ray {
    public:
        vec3 origin, direction, inverseDirection;
        Ray(const vec3& o, const vec3& d): origin(o), direction(d) {
            this->inverseDirection = vec3(1.0f/direction.x, 1.0f/direction.y, 1.0f/direction.z);
        };
};
#endif