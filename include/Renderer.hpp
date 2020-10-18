#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include "Sphere.hpp"
#include "Light.hpp"
#include "Vec3.hpp"
#include "Color.hpp"
#include "Ray.hpp"

class Renderer {
    private:
        int _width, _height;
    public:
        Renderer(int width, int height): _width(width), _height(height) {};
        std::vector<Color> render(std::vector<Sphere>& spheres, std::vector<Light>& lights);
};
#endif