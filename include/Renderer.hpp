#ifndef RENDERER_HPP
#define RENDERER_HPP

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
#include "gi/albedoCalculator.h"
#include "gi/localAC.h"
#include "gi/directAC.h"
#include "../include/utils/ProgressBar.h"

#include <thread>

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
        AlbedoCalculator* _albedoCalculator;
        unsigned int _width, _height, _sspx;
        std::string _outPath;

        DisplayType _displayType;
        Window* _window = nullptr;
        RendererType _type;
        ProgressBar _progressBar;

        int _pixelsRendered = 0;
        
        void _buildPicture();
        void _initAlbedoCalculator();
        void _initRayTracer(std::string& rtype);
        void _initLightSources(Scene& scene);
    public:
        Renderer(): _framebuffer(0, 0), _type(RendererType::LocalIllumination) {};
        void init(std::string jobPath, DisplayType displayType);
        void run();
        Color sample_pixel(unsigned int x, unsigned int y);
        ~Renderer() {
            if (this->_rayTracer != nullptr) {
                delete this->_rayTracer;
            }
            if (this->_window != nullptr) {
                delete this->_window;
            }
            if (this->_albedoCalculator != nullptr) {
                delete this->_albedoCalculator;
            }
        };
};

#endif