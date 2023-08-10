#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <iostream>

#include "Color.hpp"

class Window {
    private:
        unsigned int _width, _height;
        GLubyte* _pixelBuffer;
        GLFWwindow* _window;
    public:
        Window(unsigned int width, unsigned int height);
        Window(){};
        void drawPixel(unsigned x, unsigned y, Color color);
};
#endif