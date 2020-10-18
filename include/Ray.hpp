#ifndef RAY_HPP
#define RAY_HPP

#include "Vec3.hpp"

class Ray {
    public:
        Vec3 origin, direction;
        Ray(const Vec3& o, const Vec3& d): origin(o), direction(d) {};
};
#endif