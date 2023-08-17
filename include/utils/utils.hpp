#ifndef UTILS_HPP
#define UTILS_HPP

#include <math.h>

inline float roughness_from_exponent(float exponent) {
    return sqrtf(2.f / (exponent + 2.f));
}
inline float exponent_from_roughness(float roughness) {
    return 2.f / (roughness * roughness) - 2.f;
}

inline float random_float() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline float random_float(float min, float max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_float();
}

inline vec3 randomVec3() {
    return vec3(random_float(), random_float(), random_float());
}

inline vec3 randomVec3(double min, double max) {
    return vec3(random_float(min,max), random_float(min,max), random_float(min,max));
}

inline vec3 randomVec3OnHemisphere(vec3 n) {
    vec3 p = randomVec3(-1,1);
    p = normalize(p);
    if (dot(p, n) > 0.0) // In the same hemisphere as the normal
        return p;
    else
        return -p;
}

inline float cdot(const vec3 &a, const vec3 &b) {
    float x = a.x*b.x + a.y*b.y + a.z*b.z;
    return x < 0.0f ? 0.0f : x;
}

// Function to align 'vectorToAlign' with 'axisVector'
inline vec3 alignVectorToAxis(const glm::vec3& vectorToAlign, const glm::vec3& axis) {
    const float s = copysign(1.f, axis.z);
    const glm::vec3 w = glm::vec3(vectorToAlign.x, vectorToAlign.y, vectorToAlign.z * s);
    
    const glm::vec3 h = glm::vec3(axis.x, axis.y, axis.z+s);
    const float k = glm::dot(w, h) / (1.f + (axis.z < 0 ? -axis.z : axis.z));
    
    return k * h - w;
    /*
    const float s = copysign(1.f, axis.z);
    const vec3 w = vec3(v.x, v.y, v.z * s);
    
    const vec3 h = vec3(axis.x, axis.y, axis.z + s);
    const float k = dot(w, h) / (1.f + (axis.z < 0 ? -axis.z : axis.z));
    return k * h - w;
    */
}

#endif