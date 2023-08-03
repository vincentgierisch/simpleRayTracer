#include "../include/Renderer.hpp"

Color Renderer::getAverageColor(std::vector<Color> colors){
    Color averageColor(0, 0, 0);
    for (Color& color : colors) {
        averageColor.red += color.red;
        averageColor.green += color.green;
        averageColor.blue += color.blue;
    }
    return averageColor / colors.size();
}

Color Renderer::calculateAlbedo(HitPoint& hitpoint, Ray& r) {
    std::vector<Color> colors;
    for (Pointlight& pointlight : Scene::getInstance().Pointlights) {
        Color resultColor(0, 0, 0);
        vec3 topoint = pointlight.Position - hitpoint.x;
        vec3 wi = normalize(topoint);
        Ray shadowRay(hitpoint.x, wi);
        float distance = sqrtf(dot(topoint, topoint));
        shadowRay.setMax(distance);
        if(!this->_rayTracer->any_hit(shadowRay)) {
            //std::cout << "albedo: " << hitpoint.albedo().red << std::endl;
            vec3 c = pointlight.getPower() * hitpoint.material->brdf->f(hitpoint, wi, -r.direction) / (distance * distance);
            return glm_to_color(c);
        }
        colors.push_back(resultColor);
    }
    return this->getAverageColor(colors);
}

void Renderer::init(std::string jobPath) {
    JobData jd = JobParser::parse(jobPath);
    
    this->_sspx = jd.SamplesPerPixel;
    this->_outPath = jd.OutPath;

    // init framebuffer
    this->_framebuffer = Framebuffer(int(jd.Resolution.x), int(jd.Resolution.y));
    this->_framebuffer.clear();

    //init Scene
    Scene& scene = Scene::getInstance();
    Camera camera(jd.CameraPosition, jd.CameraDirection, jd.CameraUp, 65, jd.Resolution.x, jd.Resolution.y);
    scene.camera = camera;
    scene.DefaultBrdfType = (BrdfType) jd.DefaultBrdf;
    scene.Brdfs.emplace(std::make_pair(scene.DefaultBrdfType, BrdfFabric::getBrdf(scene.DefaultBrdfType)));


    for (JobData::PointLightData& pld : jd.PointLights) {
        Pointlight pl(pld.Position, glm_to_color(pld.Color));
        scene.Pointlights.push_back(pl);
    }

    scene.load(jd.ObjFilePath);

    if (jd.RayTracer == "bvh") {
        this->_rayTracer = new BinaryBvhRayTracer();
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
        Color sampleColor(0, 0, 0);
        Ray ray = Scene::getInstance().camera.spawnRay(x, y, vec2(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
        TriangleIntersection intersection = this->_rayTracer->closest_hit(ray);
        if (intersection.isValid()) {
            HitPoint hitPoint(intersection);
            sampleColor = this->calculateAlbedo(hitPoint, ray);
        }
        result.push_back(sampleColor);
    }

    return result;
}