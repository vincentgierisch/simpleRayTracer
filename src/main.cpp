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
#include "../include/Renderer.hpp"

int main(int argc, char *argv[]) {

    //Camera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), 65, W, H);
    //Camera camera(vec3(-13,-12, 0), vec3(1, 0, 0), vec3(0, 1, 0), 65, W, H);

    Renderer renderer;
    renderer.init("jobs/test");
    renderer.run();
    return 0;
}
/*

Color sample_pixel(Camera &camera, unsigned x, unsigned y) {
    Ray ray = camera.spawnRay(x, y);
    Color c(0, 0, 0);
    TriangleIntersection closestIntersection;
    for (Triangle& triangle: Scene::getInstance().Triangles) {
        TriangleIntersection ti;
        ti = triangle.getIntersection(Scene::getInstance().Vertices.data(), ray);
        if (ti.isValid() && ti.t<closestIntersection.t) {
            closestIntersection = ti;
            closestIntersection.triangle = &triangle;
            HitPoint hp(ti);
            c = hp.albedo();
            break;
        }
    }
    return c;
}
*/