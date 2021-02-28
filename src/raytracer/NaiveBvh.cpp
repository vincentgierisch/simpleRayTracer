#include "../../include/raytracer/NaiveBvh.hpp"

unsigned int NaiveBvhRayTracer::subdivide(std::vector<Triangle> &triangles, std::vector<Vertex> &vertices, unsigned int start, unsigned int end) {
    assert(start < end);
    // Abort recursion
    if (end - start == 1) {
        unsigned int id = this->_bvhNodes.size();
        NaiveBvhNode bvhNode;
        bvhNode.triangle = start;
        this->_bvhNodes.push_back(bvhNode);
        return id;
    }

    // get box for part scene
    AABB box;
    for(unsigned int i = start; i < end; ++i) {
        box.grow(raytracer::Helper::getBoxFromTriangle(vertices, triangles[i]));
    }

    // sort triangles by largest axis
    vec3 extent = box.getMax() - box.getMin();
    float largest = max(extent.x, max(extent.y, extent.z));
    if (largest == extent.x) {
        std::sort(triangles.data()+start, triangles.data()+end,
            [&](const Triangle &a, const Triangle &b) {return raytracer::Helper::getCenter(vertices, a).x < raytracer::Helper::getCenter(vertices, b).x; });
    } else if (largest == extent.y) {
        std::sort(triangles.data()+start, triangles.data()+end,
            [&](const Triangle &a, const Triangle &b) {return raytracer::Helper::getCenter(vertices, a).y < raytracer::Helper::getCenter(vertices, b).y; });
    } else if (largest == extent.z) {
        std::sort(triangles.data()+start, triangles.data()+end,
            [&](const Triangle &a, const Triangle &b) {return raytracer::Helper::getCenter(vertices, a).z < raytracer::Helper::getCenter(vertices, b).z; });
    }

    // cut in the middle
    int middle = start + (end-start)/2;
    unsigned int id = this->_bvhNodes.size();
    this->_bvhNodes.emplace_back();
    unsigned int left = this->subdivide(triangles, vertices, start, middle);
    unsigned int right = this->subdivide(triangles, vertices, middle, end);
    this->_bvhNodes[id].leftNode = left;
    this->_bvhNodes[id].rightNode = right;
    this->_bvhNodes[id].aabb = box;
    return id;
}

void NaiveBvhRayTracer::init() {
    std::cout << "Building BVH..." << std::endl; 
    this->_root = this->subdivide(Scene::getInstance().Triangles, Scene::getInstance().Vertices, 0, Scene::getInstance().Triangles.size());
    std::cout << "Done building BVH..." << std::endl; 
}

bool NaiveBvhRayTracer::any_hit(Ray& ray) {
    unsigned int stack[24];
    int stackPointer = 0;
    stack[0] = this->_root;

    while (stackPointer >= 0) {
        NaiveBvhNode node = this->_bvhNodes[stack[stackPointer]];
        --stackPointer;

        if (!node.isLeaf()) {
            float dist;
            if (node.aabb.doesIntersect(ray, dist)) {
                stack[++stackPointer] = node.leftNode;
                stack[++stackPointer] = node.rightNode;
            }
        } else {
            TriangleIntersection intersection = Scene::getInstance().Triangles[node.triangle].getIntersection(Scene::getInstance().Vertices.data(), ray);
            if (intersection.isValid()) {
                return true;
            }
        }
    }
    return false;
}

TriangleIntersection NaiveBvhRayTracer::closest_hit(Ray& ray) {
    TriangleIntersection closestIntersection;

    unsigned int stack[24];
    int stackPointer = 0;
    stack[0] = this->_root;

    while (stackPointer >= 0) {
        NaiveBvhNode node = this->_bvhNodes[stack[stackPointer]];
        --stackPointer;

        if (!node.isLeaf()) {
            float dist;
            if (node.aabb.doesIntersect(ray, dist)) {
                if (dist < closestIntersection.t) {
                    stack[++stackPointer] = node.leftNode;
                    stack[++stackPointer] = node.rightNode;
                }
            }
        } else {
            TriangleIntersection intersection = Scene::getInstance().Triangles[node.triangle].getIntersection(Scene::getInstance().Vertices.data(), ray);
            if (intersection.isValid() && intersection.t < closestIntersection.t) {
                closestIntersection = intersection;
                closestIntersection.triangle = &Scene::getInstance().Triangles[node.triangle];
            }
        }
    }
    return closestIntersection;
}