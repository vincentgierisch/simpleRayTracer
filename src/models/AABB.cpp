#include "../../include/models/AABB.hpp"

bool AABB::doesIntersect(Ray& ray, float& dist) {

    float tempX1 = (this->_min.x - ray.origin.x) * ray.inverseDirection.x;
    float tempX2 = (this->_max.x - ray.origin.x) * ray.inverseDirection.x;

    float txmin = (tempX1 < tempX2) ? tempX1 : tempX2;
    float txmax = (tempX2 > tempX1) ? tempX2 : tempX1;
    

    float tempY1 = (this->_min.y - ray.origin.y) * ray.inverseDirection.y;
    float tempY2 = (this->_max.y - ray.origin.y) * ray.inverseDirection.y;

    float tymin = (tempY1 < tempY2) ? tempY1 : tempY2;
    float tymax = (tempY2 > tempY1) ? tempY2 : tempY1;
    

    float tempZ1 = (this->_min.z - ray.origin.z) * ray.inverseDirection.z;
    float tempZ2 = (this->_max.z - ray.origin.z) * ray.inverseDirection.z;

    float tzmin = (tempZ1 < tempZ2) ? tempZ1 : tempZ2;
    float tzmax = (tempZ2 > tempZ1) ? tempZ2 : tempZ1;

    float maxTMin = (txmin > tymin) ? txmin : tymin;
    maxTMin = (maxTMin > tzmin) ? maxTMin : tzmin;

    float minTMax = (txmax < tymax) ? txmax : tymax;
    minTMax = (minTMax < tzmax) ? minTMax : tzmax;

    if (maxTMin > minTMax)
        return false;

    if (minTMax < FLT_MIN || maxTMin > FLT_MAX)
        return false;
    
    dist = maxTMin;
    return true;
}