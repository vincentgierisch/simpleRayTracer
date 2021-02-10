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
    Sphere sphere(Vec3(W/2, H/2, 50), 500);
    Light light(500, 0, 0);

    Framebuffer framebuffer(W, H);

    Scene scene;

    scene.load("render-data/sponza.fixed.obj", "standard");

    for (size_t y = 0; y < H; ++y)
    {
        for (size_t x = 0; x < W; ++x)
        {
            // send a ray through each pixel
            Ray ray(Vec3(x,y,0), Vec3(0,0,1));
            
            double time = 20000;

            // check for intersection
            if(sphere.intersect(ray, time)) {
                // Point of intersection
                Vec3 intersectionPoint = ray.origin + ray.direction*time;

                // Color the pixel
                Vec3 L = light - intersectionPoint;
                Vec3 N = sphere.getNormal(intersectionPoint);
                Vec3 wi = L.normalize();
                Vec3 wo = intersectionPoint - ray.origin;
                Vec3 wr = N.normalize() * 2 * (wi * N.normalize()) - wi;
                Color c = Color((white * pow(wr * wo, 0.5) * (1.5 / (2*PI)) * (wi * N.normalize()) ));
                framebuffer.add(x, y, c);
            }
        }
    }

    framebuffer.png().write("out.png");

    return 0;
}