#ifndef COLOR_HPP
#define COLOR_HPP

#include <glm/glm.hpp>

using namespace glm;


class Color {
    public:
        float red, green, blue, alpha;
        Color(): red(0), green(0), blue(0){};
        Color(float r, float g, float b): red(r), green(g), blue(b) {};
        Color(float r, float g, float b, float alpha): red(r), green(g), blue(b), alpha(alpha) {};
        Color operator * (float d) {return Color(red*d, green*d, blue*d);}
        Color operator / (float d) {return Color(red/d, green/d, blue/d);}
        Color operator * (Color c) {this->red * c.red; this->green * c.green; this->blue * c.blue; }
        void clamp255();
};

inline void Color::clamp255() {
    this->red = (this->red > 255) ? 255 : (this->red < 0) ? 0 : this->red;
    this->green = (this->green > 255) ? 255 : (this->green < 0) ? 0 : this->green;
    this->blue = (this->blue > 255) ? 255 : (this->blue < 0) ? 0 : this->blue;
}

inline float luma(const Color& rgb) {
    return (0.212671f * rgb.red) + (0.715160f * rgb.green) + (0.072169f * rgb.blue);
}

inline Color glm_to_color(const vec3& v) {
    return Color(v.x, v.y, v.z);
}

inline vec3 color_to_glm(const Color& c) {
    return vec3(c.red, c.green, c.blue);
}
#endif