#include "../include/Renderer.hpp"

void Renderer::init(std::string jobPath, DisplayType displayType) {
    this->_displayType = displayType;
    JobData jd = JobParser::parse(jobPath);
    
    this->_sspx = jd.SamplesPerPixel;
    this->_outPath = jd.OutPath;
    this->_type = (RendererType)jd.RendererType;

    this->_width = unsigned(jd.Resolution.x);
    this->_height = unsigned(jd.Resolution.y);

    if (this->_displayType == DisplayType::Live) {
        this->_window = new Window(this->_width, this->_height);
    }

    this->_progressBar = ProgressBar(0.05);

    // init framebuffer
    this->_framebuffer = Framebuffer(int(this->_width), int(this->_height));
    this->_framebuffer.clear();

    //init Scene
    Scene& scene = Scene::getInstance();
    Camera camera(jd.CameraPosition, jd.CameraDirection, jd.CameraUp, 65, int(this->_width), int(this->_height));
    scene.camera = camera;
    scene.DefaultBrdfType = (BrdfType) jd.DefaultBrdf;
    scene.Brdfs.emplace(std::make_pair(scene.DefaultBrdfType, BrdfFabric::getBrdf(scene.DefaultBrdfType)));


    for (JobData::PointLightData& pld : jd.PointLights) {
        Pointlight pl(pld.Position, pld.Color);
        scene.Pointlights.push_back(pl);
    }

    scene.load(jd.ObjFilePath);

    // check if job has overwritten materials
    for (Material& material : scene.Materials) {
        if (jd.Materials.count(material.name)) {
            JobData::MaterialData& md = jd.Materials[material.name];
            if (md.isAlbedoSet())
                material.albedo = md.getAlbedo();
            if (md.isIorSet())
                material.ior = md.getIor();
            if (md.isRoughnessSet())
                material.roughness = md.getRoughness();
            if (md.isEmissiveSet())
                material.emissive = md.getEmissive();
        }
    }

    this->_initAlbedoCalculator();
    this->_initRayTracer(jd.RayTracer);
}

void Renderer::_initRayTracer(std::string& rtype) {
    if (rtype == "bvh") {
        this->_rayTracer = new BinaryBvhRayTracer();
    } else {
        this->_rayTracer = new SeqRayTracer();
    }

    this->_rayTracer->init();
}

void Renderer::_initAlbedoCalculator() {
    switch (this->_type)
    {
        case RendererType::LocalIllumination:
            this->_albedoCalculator = new LocalAlbedoCalculator();
            break;
        case RendererType::DirectIllumination:
            this->_albedoCalculator = new DirectAlbedoCalculator();
            break;
        default:
            this->_albedoCalculator = new LocalAlbedoCalculator();
            break;
    }
}

void Renderer::run() {
    /*
    this->_framebuffer.buffer.for_each([&](unsigned x, unsigned y) {
                                        Color col = this->sample_pixel(x, y);
										this->_framebuffer.add(x, y, col);
                                        if (this->_displayType == DisplayType::Live && x == 0 && y%4 == 0) {
                                            this->_window->drawPixel(this->_framebuffer.buffer);
                                        }
    								});
    */
    this->_progressBar.init();
    std::thread raytracingThread(&Renderer::_buildPicture, this);
    if (this->_displayType == DisplayType::Live) {
		raytracingThread.detach();
        this->_window->mainLoop();
        raytracingThread.join();
    } else {
        raytracingThread.join();
    }
    // make sure Progressbar finishes
    this->_progressBar.setProgress(1);
    std::cout << "Finished rendering" << std::endl;
}

void Renderer::_buildPicture() {
    this->_framebuffer.buffer.for_each([&](unsigned x, unsigned y) {
        Color col = this->sample_pixel(x, y);
        this->_progressBar.setProgress(++this->_pixelsRendered/(float)(this->_width * this->_height));
        this->_framebuffer.add(x, y, col);
        if (this->_displayType == DisplayType::Live && x == 0) {
            this->_window->drawPixel(this->_framebuffer.buffer);
        }
    });
    this->_framebuffer.png().write(this->_outPath);
}

Color Renderer::sample_pixel(unsigned int x, unsigned int y) {
    std::vector<Color> result;

    std::vector<std::pair<vec3, vec3>> rays;

    for (unsigned int sample = 0; sample < this->_sspx; ++sample){
        Color sampleColor(0, 0, 0);
        Ray ray = Scene::getInstance().camera.spawnRay(x, y, vec2(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
        TriangleIntersection intersection = this->_rayTracer->closest_hit(ray);
        if (intersection.isValid()) {
            HitPoint hitPoint(intersection);
            if (sample == 0) {
                std::cout << hitPoint.x.x << "|" << hitPoint.x.y << "|" << hitPoint.x.z << std::endl;
            }
            float z1 = random_float(0, 0.9999f);
            float z2 = random_float(0, 0.9999f);

            float sinTheta = sqrtf(1.f-(z1*z1));
            float phi = 2.f*M_PIf*z2;

            vec3 wi = vec3(sinTheta*cosf(phi),
                            sinTheta*sinf(phi),
                            z1);
            // wi is just aligned to the basic hemisphere at (0,0), we need to align it to the normal
            const float s = copysign(1.f, hitPoint.norm.z);
            const glm::vec3 w = glm::vec3(wi.x, wi.y, wi.z * s);
            
            const glm::vec3 h = glm::vec3(hitPoint.norm.x, hitPoint.norm.y, hitPoint.norm.z+s);
            const float k = glm::dot(w, h) / (1.f + (hitPoint.norm.z < 0 ? -hitPoint.norm.z : hitPoint.norm.z));
    
            vec3 res = w - k * h;
            vec3 wi1 = wi-hitPoint.norm;


            Ray randomRay(hitPoint.x, res);

            std::pair<vec3, vec3> ray = std::make_pair(randomRay.origin, randomRay.direction);
            rays.push_back(ray);
            // sampleColor = this->_albedoCalculator->calculateAlbedo(hitPoint, ray, this->_rayTracer);
        }
        result.push_back(sampleColor);
    }
    test_vector(rays, "test.obj");

    return getAverageColor(result);
}