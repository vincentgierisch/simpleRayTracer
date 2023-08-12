#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include "../Renderer.hpp"

using namespace glm;

struct JobData {

    struct PointLightData
    {
        vec3 Position;
        vec3 Color;
    };

    class MaterialData
    {
        private: 
            vec3 _albedo;
            float _ior, _roughness;
            bool _albedoSet, _iorSet, _roughnessSet;
        public:
            MaterialData() : _albedoSet(false), _iorSet(false), _roughnessSet(false) {};

            inline void setAlbedo(vec3 albedo) {this->_albedo = albedo; this->_albedoSet = true;};
            inline void setIor(float ior) {this->_ior = ior; this->_iorSet = true;};
            inline void setRoughness(float roughness) {this->_roughness = roughness; this->_roughnessSet = true;};

            inline vec3 getAlbedo() {return this->_albedo;};
            inline float getIor() {return this->_ior;};
            inline float getRoughness() {return this->_roughness;};

            inline bool isAlbedoSet() {return this->_albedoSet;};
            inline bool isIorSet() {return this->_iorSet;};
            inline bool isRoughnessSet() {return this->_roughnessSet;};
    };
    
    short DefaultBrdf = 0;
    vec3 CameraPosition;
    vec3 CameraDirection;
    vec3 CameraUp;
    std::string RayTracer;
    std::string ObjFilePath;
    vec2 Resolution;
    unsigned int SamplesPerPixel;
    std::vector<PointLightData> PointLights;
    std::map<std::string, MaterialData> Materials;
    std::string OutPath;
    short RendererType;
};

class JobParser
{
    public:
        static JobData parse(std::string path);

};

inline std::istream& operator>>(std::istream &in, vec3& v) {
    in >> v.x >> v.y >> v.z;
    return in;
}

inline std::istream& operator>>(std::istream &in, vec2& v) {
    in >> v.x >> v.y;
    return in;
}