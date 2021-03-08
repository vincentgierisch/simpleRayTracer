#include "../../include/models/HitPoint.hpp"

HitPoint::HitPoint(float t, float beta, float gamma, Vertex& a, Vertex& b, Vertex& c, Material* m){
    // check if this is used
    this->x = (1.0f - beta - gamma)*a.pos + beta*b.pos + gamma*c.pos;
    this->textureCoordinate = (1.0f - beta - gamma)*a.textureCoordinate + beta*b.textureCoordinate + gamma*c.textureCoordinate;
    this->material = material;
    this->norm = cross(normalize(b.pos - a.pos), normalize(c.pos - a.pos));
}

HitPoint::HitPoint(TriangleIntersection& ti) {
    Vertex& a = Scene::getInstance().Vertices[ti.triangle->a];
    Vertex& b = Scene::getInstance().Vertices[ti.triangle->b];
    Vertex& c = Scene::getInstance().Vertices[ti.triangle->c];
    this->x = (1.0f - ti.beta - ti.gamma)*a.pos + ti.beta*b.pos + ti.gamma*c.pos;
    this->textureCoordinate = (1.0f - ti.beta - ti.gamma)*a.textureCoordinate + ti.beta*b.textureCoordinate + ti.gamma*c.textureCoordinate;
    this->material = &Scene::getInstance().Materials[ti.triangle->material_id];
    this->norm = normalize((a.norm + b.norm + c.norm) / 3.0f);
}

Color HitPoint::albedo() {
    if (this->material->albedo_tex) {
        return this->material->albedo_tex->sample(this->textureCoordinate);
    }
    return this->material->albedo;
}