#include "../include/Window.hpp"

Window::Window(unsigned int width, unsigned int height): _width(width), _height(height){
    // init pixel buffer
    // this->_pixelBuffer = new GLubyte[width * height * 3];

	// init sdl
	if (SDL_Init(SDL_INIT_EVERYTHING) !=0) {
		std::string errMsg(SDL_GetError());
		throw std::runtime_error("ERROR: initializing sdl: " + errMsg); 
	}

	// Create Window
	this->_window = SDL_CreateWindow("SimpleRayTracer",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width, height, 0);


	this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_PRESENTVSYNC);

	this->_texture = SDL_CreateTexture(this->_renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	// init pixelbuffer
	this->_pixelBuffer = new unsigned int[width*height];

	// set buffer to black
	for (unsigned int i = 0; i < height*width; ++i) {
		this->_pixelBuffer[i] = 0xFFFFFFFF;
	}
};

void Window::drawPixel(Buffer<Color>& buffer) {
	// color to int
	buffer.for_each([&](unsigned x, unsigned y) {
        Color color(buffer(x,y));
		color = glm_to_color(pow(clamp(color_to_glm(color), vec3(0), vec3(1)), vec3(1.0f/2.2f)) * 255.0f);
		unsigned int hexColor = (((unsigned int)color.red & 0xff) << 24) + (((unsigned int)color.green & 0xff) << 16) + (((unsigned int)color.blue & 0xff) << 8) + (0xff & 0xff);
        this->_pixelBuffer[x+y*this->_width] = hexColor;
		// out[buffer.h - y - 1][x] = png::rgb_pixel(c.red, c.green, c.blue);
	});
	
	SDL_RenderClear(this->_renderer);
	SDL_UpdateTexture(this->_texture, NULL, this->_pixelBuffer, this->_width*4);
	SDL_RenderCopy(this->_renderer, this->_texture, NULL, NULL);
	SDL_RenderPresent(this->_renderer);
};