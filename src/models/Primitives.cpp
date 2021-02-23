#include "../../include/models/Primitives.hpp"

TriangleIntersection Triangle::getIntersection(const Vertex* vertices, const Ray& ray){
    TriangleIntersection ti;

	// a
	vec3 pos = vertices[this->a].pos;
	const float a_x = pos.x;
	const float a_y = pos.y;
	const float a_z = pos.z;

	// lo
	pos = vertices[this->b].pos;
	const float &a = a_x - pos.x;
	const float &b = a_y - pos.y;
	const float &c = a_z - pos.z;
	
	// li
	pos = vertices[this->c].pos;
	const float &d = a_x - pos.x;
	const float &e = a_y - pos.y;
	const float &f = a_z - pos.z;
	
	// destination
	const float &g = ray.direction.x;
	const float &h = ray.direction.y;
	const float &i = ray.direction.z;
	
	// ergebnis
	const float &j = a_x - ray.origin.x;
	const float &k = a_y - ray.origin.y;
	const float &l = a_z - ray.origin.z;

	// (a.y - c.y) * d.z - d.y * (a.z - c.z) 
	float common1 = e*i - h*f;
	float common2 = g*f - d*i;
	float common3 = d*h - e*g;
	float M 	  = a * common1  +  b * common2  +  c * common3;
	float beta 	  = j * common1  +  k * common2  +  l * common3;

	common1       = a*k - j*b;
	common2       = j*c - a*l;
	common3       = b*l - k*c;
	float gamma   = i * common1  +  h * common2  +  g * common3;
	float tt    = -(f * common1  +  e * common2  +  d * common3);

	beta /= M;
	gamma /= M;
	tt /= M;

    if (tt > FLT_MIN && tt < FLT_MAX
        && (beta > 0) 
        && (gamma > 0) 
        && (beta + gamma) <= 1) {
            ti.t = tt;
            ti.beta = beta;
            ti.gamma = gamma;
            ti.triangle = this;
        }
    return ti;

}