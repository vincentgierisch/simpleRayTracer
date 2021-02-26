#include "../../include/models/AABB.hpp"

bool AABB::doesIntersect(Ray& ray, float& dist) {
    float tmin = FLT_MIN;
    float tmax = FLT_MAX;

    if (ray.direction.x == 0) {
        // ray points orthogonal to x axis
        if (!(this->_min.x <= ray.origin.x && this->_max.x >= ray.origin.x)) {
            // ray could not be in interval
            return false;
        }
    } else {
        tmin = (this->_min.x - ray.origin.x) / ray.direction.x;
        tmax = (this->_max.x - ray.origin.x) / ray.direction.x;

        if (tmin > tmax) {
            std::swap(tmin, tmax);
        }
    }

    if (ray.direction.y == 0) {
        // ray points orthogonal to y axis
        if (!(this->_min.y <= ray.origin.y && this->_max.y >= ray.origin.y)) {
            // ray could not be in interval
            return false;
        }
    } else {
        float tymin = (this->_min.y - ray.origin.y) / ray.direction.y;
        float tymax = (this->_max.y - ray.origin.y) / ray.direction.y;

        if (tymin > tymax) {
            std::swap(tymin, tymax);
        }

        if ((tmin > tymax) || (tymin > tmax))
            return false;

        if (tymax < FLT_MIN || tymin > FLT_MAX)
			return false;

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;
    }

    if (ray.direction.z == 0) {
        // ray points orthogonal to y axis
        if (!(this->_min.z <= ray.origin.z && this->_max.z >= ray.origin.z)) {
            // ray could not be in interval
            return false;
        }
    } else {
        float tzmin = (this->_min.z - ray.origin.z) / ray.direction.z;
        float tzmax = (this->_max.z - ray.origin.z) / ray.direction.z;

        if (tzmin > tzmax) {
            std::swap(tzmin, tzmax);
        }

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;

        if (tzmax < FLT_MIN || tzmin > FLT_MAX)
			return false;

        if (tzmin > tmin)
            tmin = tzmin;

        if (tzmax < tmax)
            tmax = tzmax;
    }

    dist = tmin;
    return true;
}