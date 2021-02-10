#ifndef COLOR_HPP
#define COLOR_HPP
class Color {
    public:
        double red, green, blue;
        Color(): red(0), green(0), blue(0){};
        Color(double r, double g, double b): red(r), green(g), blue(b) {};
        Color operator * (double d) {return Color(red*d, green*d, blue*d);}
        Color operator / (double d) {return Color(red/d, green/d, blue/d);}
        void clamp255();
};

inline void Color::clamp255() {
    this->red = (this->red > 255) ? 255 : (this->red < 0) ? 0 : this->red;
    this->green = (this->green > 255) ? 255 : (this->green < 0) ? 0 : this->green;
    this->blue = (this->blue > 255) ? 255 : (this->blue < 0) ? 0 : this->blue;
}
#endif