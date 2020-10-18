#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Vec3.hpp"
#include "Ray.hpp"
#include <cmath>

class Sphere {
    public:
        Vec3 center;
        double radius;

        Sphere(Vec3 c, double r): center(c), radius(r){};
        Vec3 getNormal(Vec3& intersection) {return (intersection - center) / radius;}
        bool intersect(Ray ray, double &t);
};
#endif