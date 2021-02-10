#pragma once

#include <png++/png.hpp>
#include <algorithm>
#include <functional>
#include "Color.hpp"
#include "Vec3.hpp"

template<typename T> 
class Buffer {
    private:
        T *data = nullptr;
    public:
        unsigned int w, h;
        Buffer(unsigned int w, unsigned int h): w(w), h(h){
            data = new T[w*h];
        }
        ~Buffer() {
            delete [] data;
        }

        T& operator()(unsigned int x, unsigned int y) { return data[y*w + x]; }
        const T& operator()(unsigned x, unsigned y) const { return data[y*w + x]; }

        Buffer& operator=(Buffer &&other) {
		    std::swap(w, other.w);
		    std::swap(h, other.h);
		    std::swap(data, other.data);
		    return *this;
	    }

        void clear(const T &to) {
            #pragma omp parallel for
            for (int i = 0; i < w*h; i++){
                data[i] = to;
            }
        }
    
        void for_each(const std::function<void(unsigned x, unsigned y)> &fn) const {
            #pragma omp parallel for
            for (unsigned y = 0; y < h; y++) {
                for (unsigned x = 0; x < w; x++) {
                    fn(x, y);
                }
            }
        }
};

class Framebuffer {
    Buffer<Color> buffer;
    public:
        Framebuffer(unsigned w, unsigned h): buffer(w, h) {};
        ~Framebuffer(){}
        void add(unsigned x, unsigned y, Color c);
        png::image<png::rgb_pixel> png() const;
};