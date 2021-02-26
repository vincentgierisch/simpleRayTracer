#include "../include/Renderer.hpp"

Color Renderer::getAverageColor(std::vector<Color> colors){
    Color averageColor(0, 0, 0);
    for (Color& color : colors) {
        averageColor.red += color.red;
        averageColor.green += color.green;
        averageColor.blue += color.blue;
    }
    return averageColor / this->_sspx;
}

void Renderer::init(std::string jobPath) {
    JobData jd = JobParser::parse(jobPath);
    
    this->_sspx = jd.SamplesPerPixel;
    this->_outPath = jd.OutPath;

    // init framebuffer
    this->_framebuffer = Framebuffer(int(jd.Resolution.x), int(jd.Resolution.y));
    this->_framebuffer.clear();

    //init Scene
    Camera camera(jd.CameraPosition, jd.CameraDirection, jd.CameraUp, 65, jd.Resolution.x, jd.Resolution.y);
    Scene::getInstance().camera = camera;

    Scene::getInstance().load(jd.ObjFilePath, "standard");

    if (jd.Algorithm == "bvh") {
        this->_rayTracer = new BvhRayTracer();
    } else {
        this->_rayTracer = new SeqRayTracer();
    }

    this->_rayTracer->init();
}

void Renderer::run() {
    this->_framebuffer.buffer.for_each([&](unsigned x, unsigned y) {
										this->_framebuffer.add(x, y, this->getAverageColor(this->sample_pixel(x, y)));
    								});

    this->_framebuffer.png().write(this->_outPath);
}

std::vector<Color> Renderer::sample_pixel(unsigned int x, unsigned int y) {
    std::vector<Color> result;

    for (unsigned int sample = 0; sample < this->_sspx; sample++){
        Ray ray = Scene::getInstance().camera.spawnRay(x, y, vec2(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
        TriangleIntersection intersection = this->_rayTracer->closest_hit(ray);
        if (intersection.isValid()) {
            HitPoint hitPoint(intersection);
            result.push_back(hitPoint.albedo());
        }
    }

    return result;
}