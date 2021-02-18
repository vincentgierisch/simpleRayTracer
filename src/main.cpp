#include <iostream>
#include <fstream>
#include <png++/png.hpp>
#include <cmath>
#include "../include/Ray.hpp"
#include "../include/Vec3.hpp"
#include "../include/Sphere.hpp"
#include "../include/Color.hpp"
#include "../include/Light.hpp"
#include "../include/FrameBuffer.hpp"
#include "../include/models/Scene.hpp"

#define PI 3.1415926

void clamp255(Color& col);

int main(int argc, char *argv[]) {
    const int W = 2000;
    const int H = 2000;

    Color white (255, 255, 255);
    Light light(500, 0, 0);

    Framebuffer framebuffer(W, H);

    Scene scene;

    scene.load("render-data/brdf-test.obj", "standard");

    for (size_t y = 0; y < H; ++y)
    {
        for (size_t x = 0; x < W; ++x)
        {
            // send a ray through each pixel
            Ray ray(Vec3(x,y,0), Vec3(0,0,1));
            
            double time = 20000;

            for (Triangle& triangle: scene.Triangles) {
                Color c(255, 0, 0);
                framebuffer.add(x, y, c);
            }
        }
    }

    framebuffer.png().write("out.png");

    return 0;
}