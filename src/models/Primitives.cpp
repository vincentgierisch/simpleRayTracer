#include "../../include/models/Primitives.hpp"

TriangleIntersection Triangle::getIntersection(const Vertex* vertices, const Ray& ray){
    TriangleIntersection ti;
	vec3 a = vertices[this->a].pos;
	vec3 b = vertices[this->b].pos;
	vec3 c = vertices[this->c].pos;

    vec3 abc = a - b;
    vec3 def = a - c;
    vec3 ghi = ray.direction;
    vec3 jkl = a - ray.origin;

    float ei_hf = (def.y * ghi.z) - (ghi.y * def.z);
    float gf_di = (ghi.x * def.z) - (def.x * ghi.z);
    float dh_eg = (def.x * ghi.y) - (def.y * ghi.x);

    float ak_jb = (abc.x * jkl.y) - (jkl.x * abc.y);
    float jc_al = (jkl.x * abc.z) - (abc.x * jkl.z);
    float bl_kc = (abc.y * jkl.z) - (jkl.y * abc.z);

    float beta = (jkl.x * ei_hf) + (jkl.y * gf_di) + (jkl.z * dh_eg);
    float gamma = (ghi.z * ak_jb) + (ghi.y * jc_al) + (ghi.x * bl_kc);
    float tt = -((def.z * ak_jb) + (def.y * jc_al) + (def.x * bl_kc));
    float M = (abc.x * ei_hf) + (abc.y * gf_di) + (abc.z * dh_eg);


	beta /= M;
	gamma /= M;
	tt /= M;

    if (tt > ray.tMin && tt < ray.tMax
        && (beta > 0) 
        && (gamma > 0) 
        && (beta + gamma) <= 1) {
            ti.t = tt;
            ti.beta = beta;
            ti.gamma = gamma;
            ti.triangle = this;
            return ti;
    }
    ti.t = FLT_MAX;
    return ti;
}