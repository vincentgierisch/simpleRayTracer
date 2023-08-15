#ifndef UTILS_HPP
#define UTILS_HPP

#include <math.h>

inline float roughness_from_exponent(float exponent) {
    return sqrtf(2.f / (exponent + 2.f));
}
inline float exponent_from_roughness(float roughness) {
    return 2.f / (roughness * roughness) - 2.f;
}

inline float random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline float random_double(float min, float max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

inline vec3 randomVec3() {
    return vec3(random_double(), random_double(), random_double());
}

inline vec3 randomVec3(double min, double max) {
    return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

inline vec3 randomVec3OnHemisphere(vec3 n) {
    vec3 p = randomVec3(-1,1);
    p = normalize(p);
    if (dot(p, n) > 0.0) // In the same hemisphere as the normal
        return p;
    else
        return -p;
}

#endif