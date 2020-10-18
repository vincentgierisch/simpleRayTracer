#include "../include/Sphere.hpp"

// t is distance
bool Sphere::intersect(Ray ray, double &t) {
    Vec3 oc = ray.origin - this->center;
    double b = 2*(oc * ray.direction);
    
    double c = (oc * oc) - this->radius * this->radius;
    double disc = b*b - c*4;

    if (disc < 0) {
        return false;
    } else {
        disc = sqrt(disc);
        double t0 = -b-disc;
        double t1 = -b+disc;

        t = (t0 < t1) ? t0 : t1;
        return true;
    }
}