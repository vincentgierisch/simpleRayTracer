#include "../include/Renderer.hpp"

void Renderer::init(std::string jobPath, DisplayType displayType) {
    this->_displayType = displayType;
    JobData jd = JobParser::parse(jobPath);
    
    this->_sspx = jd.SamplesPerPixel;
    this->_outPath = jd.OutPath;
    this->_type = (RendererType)jd.RendererType;

    if (this->_displayType == DisplayType::Live) {
        this->_window = new Window(unsigned(jd.Resolution.x), unsigned(jd.Resolution.y));
    }

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

    // check if job has overwritten materials
    for (Material& material : scene.Materials) {
        if (jd.Materials.count(material.name)) {
            JobData::MaterialData& md = jd.Materials[material.name];
            if (md.isAlbedoSet())
                material.albedo = glm_to_color(md.getAlbedo());
            if (md.isIorSet())
                material.ior = md.getIor();
            if (md.isRoughnessSet())
                material.roughness = md.getRoughness();
        }
    }

    if (jd.RayTracer == "bvh") {
        this->_rayTracer = new BinaryBvhRayTracer();
    } else {
        this->_rayTracer = new SeqRayTracer();
    }

    
    if (this->_type == RendererType::LocalIllumination) {
        this->_albedoCalculator = new LocalAlbedoCalculator();
    } else {
        // fallback
        this->_albedoCalculator = new LocalAlbedoCalculator();
    }

    this->_rayTracer->init();
}

void Renderer::run() {
    this->_framebuffer.buffer.for_each([&](unsigned x, unsigned y) {
                                        Color col = this->sample_pixel(x, y);
										this->_framebuffer.add(x, y, col);
                                        if (this->_displayType == DisplayType::Live && x == 0 && y%4 == 0) {
                                            this->_window->drawPixel(this->_framebuffer.buffer);
                                        }
    								});
    this->_framebuffer.png().write(this->_outPath);
    if (this->_displayType == DisplayType::Live) {
        this->_window->waitTillClose();
    }
}

Color Renderer::sample_pixel(unsigned int x, unsigned int y) {
    std::vector<Color> result;

    for (unsigned int sample = 0; sample < this->_sspx; ++sample){
        Color sampleColor(0, 0, 0);
        Ray ray = Scene::getInstance().camera.spawnRay(x, y, vec2(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
        TriangleIntersection intersection = this->_rayTracer->closest_hit(ray);
        if (intersection.isValid()) {
            HitPoint hitPoint(intersection);
            sampleColor = this->_albedoCalculator->calculateAlbedo(hitPoint, ray, this->_rayTracer);
        }
        result.push_back(sampleColor);
    }

    return getAverageColor(result);
}