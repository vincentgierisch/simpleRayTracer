#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdexcept>
#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "Color.hpp"
#include "FrameBuffer.hpp"

class Window {
    private:
        unsigned int _width, _height;
        unsigned int* _pixelBuffer;

        SDL_Window* _window;
        SDL_Renderer* _renderer;
        SDL_Texture* _texture;
    public:
        Window(unsigned int width, unsigned int height);
        Window(){};
        void drawPixel(Buffer<Color>& buffer);
        ~Window() {
            delete[] this->_pixelBuffer;
        };
};
#endif