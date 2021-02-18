#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>

class Vec3{
    public: 
        double x,y,z;
        Vec3(){x=y=z=0;}
        Vec3(double x, double y, double z): x(x), y(y), z(z) {};
        Vec3 operator - (Vec3 v) {return Vec3(x-v.x, y-v.y, z-v.z);}
        Vec3 operator + (Vec3 v) {return Vec3(x+v.x, y+v.y, z+v.z);}
        Vec3 operator * (double d) {return Vec3(x*d, y*d, z*d);}
        Vec3 operator / (double d) {return Vec3(x/d, y/d, z/d);}

        double dot(Vec3 v) {return x*v.x + y*v.y + z*v.z;}

        double getLength();
        Vec3 normalize();
};
#endif