#pragma once

#include <glm/glm.hpp>
#include "../Ray.hpp"

using namespace glm;

class Camera{
    private:
        vec3 _position, _direction, _up;
        vec3 _u, _v;
        float _aspect, _pw, _ph;
    public:
        int Width, Height;
        Camera(){};
        Camera(vec3 position, vec3 direction, vec3 up, float phi,
                int width, int height): _position(position),
                _direction(direction), _up(up),
                Width(width), Height(height) {
                    this->_u = cross(direction, up);
                    this->_v = cross(_u, direction);
                    this->_aspect = float(width) / height;
                    this->_ph = 2.0f * tanf((phi * 180.0f) / (2.0f * M_PI));
                    this->_ph = tanf(float(M_PI) * phi * 0.5f / 180.0f);
                    this->_pw = this->_aspect * this->_ph;
        }
        
        inline Ray spawnRay(int x, int y, vec2 rand) {
            //float dx = (((2.0f * (float(x) + 0.5f)) / (this->Width)) - 1.0f) * this->_pw;
            //float dy = (((2.0f * (float(y) + 0.5f)) / (this->Height)) - 1.0f) * this->_ph;

            	float u = ((float)x+0.5f+rand.x)/(float)this->Width * 2.0f - 1.0f;	// \in (-1,1)
                float v = ((float)y+0.5f+rand.y)/(float)this->Height * 2.0f - 1.0f;
                u = this->_pw * u;	// \in (-near_w,near_w)
                v = this->_ph * v;

            return Ray(this->_position, normalize(this->_direction + this->_u*u + this->_v*v));
        }
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsequence-point"
inline void test_camrays(Camera &camera, int stride) {
	std::ofstream out("test.obj");
	int i = 1;
	for (int y = 0; y < camera.Height; y += stride)
		for (int x = 0; x < camera.Width; x += stride) {
			Ray ray = camera.spawnRay(x, y, vec2(0, 0));
			out << "v " << ray.origin.x << " " << ray.origin.y << " " << ray.origin.z << std::endl;
			out << "v " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << std::endl;
			out << "l " << i++ << " " << i++ << std::endl;
		}
}
#pragma GCC diagnostic pop