#include <iostream>
#include <fstream>
#include "../include/Ray.hpp"
#include "../include/Vec3.hpp"
#include "../include/Sphere.hpp"
#include "../include/Color.hpp"
#include "../include/Light.hpp"

void clamp255(Color& col);

int main(int argc, char *argv[]) {
    const int W = 500;
    const int H = 500;

    std::ofstream out("out.ppm");
    out << "P3\n" << W << '\n' << H << '\n' << "255\n";

    Color framebuffer[H][W];

    Color white (255, 255, 255);
    Sphere sphere(Vec3(W/2, H/2, 50), 50);
    Light light(0, 0, 50);

    for(int y = 0; y < H; y++) {
        for(int x = 0; x < W; x++) {
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
                double dt = L.normalize() * N.normalize();

                framebuffer[y][x] = white * dt;
                clamp255(framebuffer[y][x]);                
            }

            out << (int) framebuffer[y][x].red << ' ';
            out << (int) framebuffer[y][x].green << ' ';
            out << (int) framebuffer[y][x].blue << '\n';
        }
    }

    out.close();

    return 0;
}


void clamp255(Color& col) {
  col.red = (col.red > 255) ? 255 : (col.red < 0) ? 0 : col.red;
  col.green = (col.green > 255) ? 255 : (col.green < 0) ? 0 : col.green;
  col.blue = (col.blue > 255) ? 255 : (col.blue < 0) ? 0 : col.blue;
}