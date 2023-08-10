#include "../include/Window.hpp"

Window::Window(unsigned int width, unsigned int height): _width(width), _height(height){
    // init pixel buffer
    this->_pixelBuffer = new GLubyte[width * height * 3];

    // init window
    if(!glfwInit()) {
		std::string description;
        const char* pDescription = description.c_str();
		int code = glfwGetError(&pDescription);
        throw std::runtime_error("Failed to initializeGLFW: " + code);
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	

	// Create Window
	this->_window = glfwCreateWindow(width, height, "SimpleRayTracer", NULL, NULL);
	if(this->_window == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to open GLFW window");
	}
	glfwMakeContextCurrent(this->_window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
	  /* Problem: glewInit failed, something is seriously wrong. */
        glfwTerminate();
        throw std::runtime_error("Error while init glew");
		
	}
	glfwSetInputMode(this->_window, GLFW_STICKY_KEYS, GL_TRUE);
	glViewport(0, 0, width, height);
};

void Window::drawPixel(unsigned x, unsigned y, Color color) {
    int position = (x + y * this->_width) * 3;
    this->_pixelBuffer[position] = 1;
    this->_pixelBuffer[position + 1] = color.green / 255.0f;
    this->_pixelBuffer[position + 2] = color.blue / 255.0f;

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(this->_width, this->_height, GL_RGB, GL_UNSIGNED_BYTE, this->_pixelBuffer);
    glfwSwapBuffers(this->_window); 
    glfwPollEvents();
};