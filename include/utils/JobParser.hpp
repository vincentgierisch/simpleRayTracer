#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace glm;

struct JobData {

    struct PointLightData
    {
        vec3 Position;
        vec3 Color;
    };
    

    vec3 CameraPosition;
    vec3 CameraDirection;
    vec3 CameraUp;
    std::string ObjFilePath;
    vec2 Resolution;
    unsigned int SamplesPerPixel;
    std::vector<PointLightData> PointLights;
    std::string OutPath;
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