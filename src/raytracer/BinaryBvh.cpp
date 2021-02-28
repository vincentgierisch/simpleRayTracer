#include "../../include/raytracer/BinaryBvh.hpp"

unsigned int BinaryBvhRayTracer::subdivide(std::vector<Triangle> &triangles, std::vector<Vertex> &vertices, unsigned int start, unsigned int end) {
    assert(start < end);
    // Abort recursion
    if (end - start == 1) {
        unsigned int id = this->_bvhNodes.size();
        BinaryBvhNode bvhNode;
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
    
	unsigned int middleIndex = start;
	Triangle* current_left  = triangles.data() + start;
	Triangle* current_right = triangles.data() + end-1;

	auto sort_sm = [&](auto getAxisToCut) {
		float spatial_median = getAxisToCut(box.getMin() + (box.getMax() - box.getMin())*0.5f);
		while (current_left < current_right) {

            // Triangles to the left
			while (getAxisToCut(raytracer::Helper::getCenter(vertices, *current_left)) <= spatial_median && current_left < current_right) {
				current_left++;
				middleIndex++;
			}

            // Triangles to the left
			while (getAxisToCut(raytracer::Helper::getCenter(vertices, *current_right)) > spatial_median && current_left < current_right) {
				current_right--;
			}


			if (getAxisToCut(raytracer::Helper::getCenter(vertices, *current_left)) > getAxisToCut(raytracer::Helper::getCenter(vertices, *current_right)) && current_left < current_right) {
				std::swap(*current_left, *current_right);
			}
		}

        // Corner case
        // If not handled, endless loop would occur
		if (middleIndex == start || middleIndex == end-1)  {
			std::sort(triangles.data()+start, triangles.data()+end,
			  [&](const Triangle &a, const Triangle &b) { return getAxisToCut(raytracer::Helper::getCenter(vertices, a)) < getAxisToCut(raytracer::Helper::getCenter(vertices, b)); });
			  middleIndex = start + (end-start)/2;
		}
	};
	
    // Check on which axis we want to cut
	if (largest == extent.x) {
		sort_sm([](const vec3 &v) { return v.x; });
	}
	else if (largest == extent.y) {
		sort_sm([](const vec3 &v) { return v.y; });
	}
	else {
		sort_sm([](const vec3 &v) { return v.z; });
	}

    // cut in the middle
    unsigned int id = this->_bvhNodes.size();
    this->_bvhNodes.emplace_back();
    unsigned int left = this->subdivide(triangles, vertices, start, middleIndex);
    unsigned int right = this->subdivide(triangles, vertices, middleIndex, end);
    this->_bvhNodes[id].leftNode = left;
    this->_bvhNodes[id].rightNode = right;

    for (unsigned int i = start; i < middleIndex; ++i) {
        this->_bvhNodes[id].leftBox.grow(raytracer::Helper::getBoxFromTriangle(vertices, triangles[i]));
    }
    for (unsigned int i = middleIndex; i < end; ++i) {
        this->_bvhNodes[id].rightBox.grow(raytracer::Helper::getBoxFromTriangle(vertices, triangles[i]));
    }
    return id;
}

void BinaryBvhRayTracer::init() {
    std::cout << "Building BVH..." << std::endl; 
    this->_root = this->subdivide(Scene::getInstance().Triangles, Scene::getInstance().Vertices, 0, Scene::getInstance().Triangles.size());
    std::cout << "Done building BVH..." << std::endl; 
}

bool BinaryBvhRayTracer::any_hit(Ray& ray) {
    unsigned int stack[24];
    int stackPointer = 0;
    stack[0] = this->_root;

    while (stackPointer >= 0) {
        BinaryBvhNode node = this->_bvhNodes[stack[stackPointer]];
        --stackPointer;

        if (!node.isLeaf()) {
            float leftDist, rightDist;
            bool hitLeft = node.leftBox.doesIntersect(ray, leftDist);
            bool hitRight = node.rightBox.doesIntersect(ray, rightDist);

            if (hitLeft && hitRight) {
                if (leftDist < rightDist) {
                    stack[++stackPointer] = node.rightNode;
                    stack[++stackPointer] = node.leftNode;
                } else {
                    stack[++stackPointer] = node.leftNode;
                    stack[++stackPointer] = node.rightNode;
                }
            } else if (hitLeft) {
                stack[++stackPointer] = node.leftNode;
            } else if (hitRight) {
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

TriangleIntersection BinaryBvhRayTracer::closest_hit(Ray& ray) {
    TriangleIntersection closestIntersection;

    unsigned int stack[24];
    int stackPointer = 0;
    stack[0] = this->_root;

    while (stackPointer >= 0) {
        BinaryBvhNode node = this->_bvhNodes[stack[stackPointer]];
        --stackPointer;

        if (!node.isLeaf()) {
            float leftDist, rightDist;
            bool hitLeft = node.leftBox.doesIntersect(ray, leftDist);
            bool hitRight = node.rightBox.doesIntersect(ray, rightDist);

            if (hitLeft && hitRight) {
                if (leftDist < rightDist) {
                    stack[++stackPointer] = node.rightNode;
                    stack[++stackPointer] = node.leftNode;
                } else {
                    stack[++stackPointer] = node.leftNode;
                    stack[++stackPointer] = node.rightNode;
                }
            } else if (hitLeft) {
                stack[++stackPointer] = node.leftNode;
            } else if (hitRight) {
                stack[++stackPointer] = node.rightNode;
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