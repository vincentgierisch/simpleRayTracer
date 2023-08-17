#include "../../include/utils/JobParser.hpp"

JobData JobParser::parse(std::string path) {
    JobData jd;

    // bit hacky
    std::string materialName = "";
    JobData::MaterialData md = JobData::MaterialData();

    std::ifstream infile(path);
    while (!infile.eof()) {
        std::string line, command;
        std::getline(infile, line);
        
        std::istringstream in(line);

        in >> command;
        if (command == "at") {
            in >> jd.CameraPosition;
        } else if (command == "look"){
            in >> jd.CameraDirection;
        } else if (command == "up"){
            in >> jd.CameraUp;
        } else if (command == "load"){
            in >> jd.ObjFilePath;
        } else if (command == "outfile"){
            in >> jd.OutPath;
        } else if (command == "sppx"){
            in >> jd.SamplesPerPixel;
        } else if (command == "resolution"){
            in >> jd.Resolution;
        } else if (command == "pointlight"){
            vec3 position, color;
            std::string cmd;
            in >> cmd;
            if (cmd == "pos") {
                in >> position;
            } else {
                throw std::runtime_error("ERROR: unknown pointlight command: " + cmd);
            }
            in >> cmd;
            if (cmd == "col") {
                in >> color;
            } else {
                throw std::runtime_error("ERROR: unknown pointlight command: " + cmd);
            }
            JobData::PointLightData pld;
            pld.Position = position;
            pld.Color = color;
            jd.PointLights.push_back(pld);
        } else if (command == "material") {
            std::string cmd;
            in >> cmd;
            if (cmd == "end") {
                if (materialName == "") {
                    throw std::runtime_error("ERROR: no material set: " + cmd);
                }
                jd.Materials[materialName] = md;
                materialName = "";
            } else if (cmd == "select") {
                md = JobData::MaterialData();
                std::string name;
                in >> name;
                materialName = name;
            } else if (cmd == "albedo") {
                vec3 albedo;
                in >> albedo;
                md.setAlbedo(albedo);
            } else if (cmd == "ior") {
                float ior;
                in >> ior;
                md.setIor(ior);
            } else if (cmd == "roughness") {
                float roughness;
                in >> roughness;
                md.setRoughness(roughness);
            } else if (cmd == "emissive") {
                vec3 emissive;
                in >> emissive;
                md.setEmissive(emissive);
            } else {
                throw std::runtime_error("ERROR: unknown material command: " + cmd);
            }
        } else if (command == "raytracer") {
            in >> jd.RayTracer;
        } else if (command == "default-brdf") {
            in >> jd.DefaultBrdf;
        } else if (command == "rendererType") {
            in >> jd.RendererType;
        } else if (command == "") {
        } else if (command[0] == '#') {
        } else {
            throw std::runtime_error("ERROR: unknown job command: " + command);
        }
    }

    return jd;
}