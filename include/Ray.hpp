#ifndef RAY_HPP
#define RAY_HPP

#include <glm/glm.hpp>

using namespace glm;

class Ray {
    public:
        vec3 origin, direction, inverseDirection;
        float tMax = FLT_MAX;
        float tMin = 1e-4f;
        Ray(const vec3& o, const vec3& d): origin(o), direction(d) {
            this->inverseDirection = vec3(1.0f/direction.x, 1.0f/direction.y, 1.0f/direction.z);
        };
        inline void setMax(float t) {
            this->tMax = t - 1e-4f;
        };
        inline void setMin(float t) {
            this->tMin = t;
        };
};
#endif