#include "../../include/raytracer/Bvh.hpp"

unsigned int BvhRayTracer::subdivide(std::vector<Triangle> &triangles, std::vector<Vertex> &vertices, unsigned int start, unsigned int end) {
    assert(start < end);
    // Abort recursion
    if (end - start == 1) {
        unsigned int id = this->_bvhNodes.size();
        BvhNode bvhNode;
        bvhNode.triangle = start;
        this->_bvhNodes.push_back(bvhNode);
        return id;
    }

    // get box for part scene
    AABB box;
    for(unsigned int i = start; i < end; ++i) {
        box.grow(this->getBoxFromTriangle(vertices, triangles[i]));
    }

    // sort triangles by largest axis
    vec3 extent = box.getMax() - box.getMin();
    float largest = max(extent.x, max(extent.y, extent.z));
    if (largest == extent.x) {
        std::sort(triangles.data()+start, triangles.data()+end,
            [&](const Triangle &a, const Triangle &b) {return this->getCenter(vertices, a).x < this->getCenter(vertices, b).x; });
    } else if (largest == extent.y) {
        std::sort(triangles.data()+start, triangles.data()+end,
            [&](const Triangle &a, const Triangle &b) {return this->getCenter(vertices, a).y < this->getCenter(vertices, b).y; });
    } else if (largest == extent.z) {
        std::sort(triangles.data()+start, triangles.data()+end,
            [&](const Triangle &a, const Triangle &b) {return this->getCenter(vertices, a).z < this->getCenter(vertices, b).z; });
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

AABB BvhRayTracer::getBoxFromTriangle(std::vector<Vertex> &vertices, Triangle& triangle) {
    AABB box;
    box.grow(vertices[triangle.a].pos);
    box.grow(vertices[triangle.b].pos);
    box.grow(vertices[triangle.c].pos);
    return box;
}

vec3 BvhRayTracer::getCenter(std::vector<Vertex> &vertices, const Triangle& triangle) {
    return (
        vertices[triangle.a].pos +
        vertices[triangle.b].pos +
        vertices[triangle.c].pos
    ) * 0.333333f;
}


void BvhRayTracer::init() {
    std::cout << "Building BVH..." << std::endl; 
    this->_root = this->subdivide(Scene::getInstance().Triangles, Scene::getInstance().Vertices, 0, Scene::getInstance().Triangles.size());
    std::cout << "Done building BVH..." << std::endl; 
}

TriangleIntersection BvhRayTracer::any_hit(Ray& ray) {

}

TriangleIntersection BvhRayTracer::closest_hit(Ray& ray) {
    TriangleIntersection closestIntersection;

    unsigned int stack[24];
    int stackPointer = 0;
    stack[0] = this->_root;

    while (stackPointer >= 0) {
        BvhNode node = this->_bvhNodes[stack[stackPointer--]];

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