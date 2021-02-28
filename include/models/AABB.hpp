#pragma once

#include "../Ray.hpp"
#include <algorithm>
#include <glm/glm.hpp>

using namespace glm;


// Axis Aligned Bounding Box

class AABB {
    private:
        vec3 _min, _max;
    public:
        AABB(): _min(vec3(0)), _max(vec3(0)) {};
        AABB(vec3 min, vec3 max): _min(min), _max(max) {};
        bool doesIntersect(Ray& ray, float& dist);
        inline vec3 getMin() { return this->_min; };
        inline vec3 getMax() { return this->_max; };
        inline void grow (vec3 v) {
            this->_min = min(v, this->_min);
            this->_max = max(v, this->_max);
        }
        inline void grow (AABB aabb) {
            this->_min = min(aabb.getMin(), this->_min);
            this->_max = max(aabb.getMax(), this->_max);
        }
};