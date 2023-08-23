#ifndef UTILS_HPP
#define UTILS_HPP

#include <math.h>

inline float roughness_from_exponent(float exponent) {
    return sqrtf(2.f / (exponent + 2.f));
}
inline float exponentFromRoughness(float roughness) {
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

inline vec2 randomVec2(float min, float max) {
    return vec2(random_float(min, max), random_float(min, max));
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

// Tom Duff (tangent space transformation)
inline vec3 toWorldSpace(const vec3& vec, const vec3& norm) {
    // wi is just aligned to the basic hemisphere at (0,0), we need to align it to the normal
    float sign = std::copysignf (1.0f , norm.z);
    float a = -1.0f / (sign + norm.z);
    float b = norm.x * norm.y * a ;
    vec3 tangent = vec3(1.0f + sign * norm.x * norm.x * a, sign * b, -sign * norm.x);
    vec3 bitangent = vec3(b, sign + norm.y * norm.y * a, - norm.y);

    return tangent * vec.x + bitangent * vec.y + norm * vec.z;
}

#endif