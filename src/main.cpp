#include <iostream>
#include <fstream>
#include <png++/png.hpp>
#include <cmath>
#include "../include/Ray.hpp"
#include "../include/Color.hpp"
#include "../include/FrameBuffer.hpp"
#include "../include/models/Scene.hpp"
#include "../include/models/HitPoint.hpp"
#include "../include/gi/Camera.hpp"

Color sample_pixel (Camera&, unsigned, unsigned);

int main(int argc, char *argv[]) {
    const int W = 640;
    const int H = 360; 

    //Camera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), 65, W, H);
    Camera camera(vec3(-2.6908, 1.7537, -0.050779), vec3(0, 0, -1), vec3(0, 1, 0), 65, W, H);
    // vec3 position, vec3 direction, vec3 up, float phi, int width, int height
    Framebuffer framebuffer(W, H);

    framebuffer.clear();
    
    Scene::getInstance().load("render-data/brdf-test-noal.obj", "standard");
    
    framebuffer.buffer.for_each([&](unsigned x, unsigned y) {
										framebuffer.add(x, y, sample_pixel(camera, x, y));
    								});

    framebuffer.png().write("out.png");
    return 0;
}

Color sample_pixel(Camera &camera, unsigned x, unsigned y) {
    Ray ray = camera.spawnRay(x, y);
    Color c(0, 0, 0);
    for (Triangle& triangle: Scene::getInstance().Triangles) {
        TriangleIntersection ti;
        ti = triangle.getIntersection(Scene::getInstance().Vertices.data(), ray);
        if (ti.isValid()) {
            HitPoint hp(ti);
            c = hp.albedo();
            break;
        }
    }
    return c;
}