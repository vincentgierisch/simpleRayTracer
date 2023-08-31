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

inline float absdot(const vec3 &a, const vec3 &b) {
    float x = a.x*b.x + a.y*b.y + a.z*b.z;
    return x < 0.0f ? -x : x;
}

inline std::pair<vec3, vec3> randomPointOnTriangle(const Vertex& a, const Vertex& b, const Vertex& c) {
    vec2 p = randomVec2(0.f, 0.9999f);

    float sqrta = sqrtf(p.x);
    vec2 st(1.f-sqrta, p.y*sqrta);
    vec3 wi = (1.0f-st.x-st.y)*a.pos + st.x*b.pos + st.y*c.pos;
    vec3 n = (1.0f-st.x-st.y)*a.norm + st.x*b.norm + st.y*c.norm;

    return {wi, n};
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsequence-point"
inline void test_vector(std::vector<std::pair<vec3, vec3>>& rays, std::string outPath) {
	std::ofstream out(outPath);
	unsigned int i = 1;

    for (std::pair<vec3, vec3>& ray : rays) {
        out << "v " << ray.first.x << " " << ray.first.y << " " << ray.first.z << std::endl;
        out << "v " << ray.second.x << " " << ray.second.y << " " << ray.second.z << std::endl;
        out << "l " << i++ << " " << i++ << std::endl;
    }
}
#pragma GCC diagnostic pop

#endif