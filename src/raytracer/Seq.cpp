#include "../../include/raytracer/Seq.hpp"

void SeqRayTracer::init(){
    return;
}

bool SeqRayTracer::any_hit(Ray& ray){
    TriangleIntersection intersection;
    for (Triangle& triangle: Scene::getInstance().Triangles) {
        intersection = triangle.getIntersection(Scene::getInstance().Vertices.data(), ray);
        if (intersection.isValid()) {
            return true;
        }
    }
    return false;
}

TriangleIntersection SeqRayTracer::closest_hit(Ray& ray){
    TriangleIntersection closestIntersection;
    for (Triangle& triangle: Scene::getInstance().Triangles) {
        TriangleIntersection intersection = triangle.getIntersection(Scene::getInstance().Vertices.data(), ray);
        if (intersection.isValid() && closestIntersection.t > intersection.t) {
            closestIntersection = intersection;
            closestIntersection.triangle = &triangle;
        }
    }
    return closestIntersection;
}