#pragma once

#include <string>
#include <math.h>
#include "../Color.hpp"

class Texture {
    private:
        std::string _path;
    public:
        unsigned int width, height;
        std::string name; // for identification purposes
        Color* Texel = nullptr;

        const Color& sample(vec2 c) const {
            float u = c.x - floor(c.x);
            float v = c.y - floor(c.y);

            int x = (int)(u*width+0.5f);
            int y = (int)(v*height+0.5f);

            if (x == width) x = 0;
            if (y == height) y = 0;

            return Texel[y*width+x];
        }

        ~Texture() {
            if (this->Texel != nullptr) {
                delete[] this->Texel;
            }
        }

};