#include "../include/RayTracer.hpp"

TriangleIntersection RayTracer::any_hit(Ray& ray){
    TriangleIntersection intersection;
    for (Triangle& triangle: Scene::getInstance().Triangles) {
        intersection = triangle.getIntersection(Scene::getInstance().Vertices.data(), ray);
        if (intersection.isValid()) {
            return intersection;
        }
    }
    return intersection;
}

TriangleIntersection RayTracer::closest_hit(Ray& ray){
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