#include "../include/Vec3.hpp"

Vec3 Vec3::normalize() {
    double length = this->getLength();
    return Vec3(x/length, y/length, z/length);
}

double Vec3::getLength() {
    return sqrt(x*x+y*y+z*z);
}