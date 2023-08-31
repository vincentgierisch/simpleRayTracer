#ifndef RENDERER_CONTAINER_H
#define RENDERER_CONTAINER_H

#include "../models/Scene.hpp"
#include "ImportanceSamplingType.hpp"
#include "../RayTracer.hpp"

struct RendererContainer {
    Scene* scene;
    ImportanceSamplingType samplingType;
    RayTracer* rayTracer;
};
#endif