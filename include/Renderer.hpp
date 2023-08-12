#pragma once

#include <string>
#include <glm/glm.hpp>
#include "utils/JobParser.hpp"
#include "models/HitPoint.hpp"
#include "models/Scene.hpp"
#include "models/Pointlight.hpp"
#include "Color.hpp"
#include "RayTracer.hpp"
#include "raytracer/Seq.hpp"
#include "raytracer/BinaryBvh.hpp"
#include "raytracer/NaiveBvh.hpp"
#include "FrameBuffer.hpp"
#include "utils/DisplayType.hpp"
#include <vector>
#include "Window.hpp"

using namespace glm;

typedef enum RendererType {
    LocalIllumination = 0,
    DirectIllumination = 1,
    GlobalIllumination = 2

} RendererType;

class Renderer {
    private:
        Framebuffer _framebuffer;
        RayTracer* _rayTracer;
        unsigned int _width, _height, _sspx;
        std::string _outPath;
        Color getAverageColor(std::vector<Color> colors);
        Color calculateAlbedo(HitPoint& hitpoint, Ray& wo);

        DisplayType _displayType;
        Window* _window = nullptr;
        RendererType _type;
    public:
        Renderer(): _framebuffer(0, 0), _type(RendererType::LocalIllumination) {};
        void init(std::string jobPath, DisplayType displayType);
        void run();
        std::vector<Color> sample_pixel(unsigned int x, unsigned int y);
        ~Renderer() {
            if (this->_rayTracer != nullptr) {
                delete this->_rayTracer;
            }
            if (this->_window != nullptr) {
                delete this->_window;
            }
        };
};