#ifndef RAY_HPP
#define RAY_HPP

#include <glm/glm.hpp>

using namespace glm;

class Ray {
    public:
        vec3 origin, direction;
        Ray(const vec3& o, const vec3& d): origin(o), direction(d) {};
};
#endif