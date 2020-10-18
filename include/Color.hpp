#ifndef COLOR_HPP
#define COLOR_HPP
class Color {
    public:
        double red, green, blue;
        Color(): red(0), green(0), blue(0){};
        Color(double r, double g, double b): red(r), green(g), blue(b) {};
        Color operator * (double d) {return Color(red*d, green*d, blue*d);}
};
#endif