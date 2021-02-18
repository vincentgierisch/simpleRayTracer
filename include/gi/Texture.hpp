#pragma once

#include <string>
#include <math.h>
#include "../Color.hpp"

class Texture {
    private:
        std::string _path;
    public:
        unsigned int w, h;
        Color* Texel = nullptr;

        const Color& sample(float u, float v) const {
            u = u - floor(u);
            v = v - floor(v);

            int x = (int)(u*w+0.5f);
            int y = (int)(v*h+0.5f);

            if (x == w) x = 0;
            if (y == h) y = 0;

            return Texel[y*w+x];
        }

        ~Texture() {
            delete[] Texel;
        }

};