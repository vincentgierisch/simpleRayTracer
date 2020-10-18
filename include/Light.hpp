#ifndef LIGHT_HPP
#define LIGHT_HPP
class Light: public Vec3 {
    public:
        Light(double x, double y, double z): Vec3(x, y, z) {};
        Light(): Vec3() {};
};
#endif
