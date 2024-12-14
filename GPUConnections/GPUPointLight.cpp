#include "GPUConnections/GPUPointLight.hh"

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c):PointLight(posicio, Ia, Id, Is, a, b, c) {
}

void GPUPointLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO Pràctica 2: Fase 1: Pas 2: enviar les llums a la GPU
}

void GPUPointLight::bindMemory(gl_IDLight *id) {
    #define POINTLIGHT_TYPE 0

    // Bind Ia, Id, Is
    GPULight::bindMemory(id);

    // Bind variables específiques
    glUniform1i(id->type, POINTLIGHT_TYPE);
    glUniform4fv(id->pos, 1, vec4(pos, 1));
    glUniform1f(id->a, a);
    glUniform1f(id->b, b);
    glUniform1f(id->c, c);
}

vec3 GPUPointLight::vectorL(vec3 point) {
    return PointLight::vectorL(point);
}

float GPUPointLight::attenuation(vec3 point) {
    return PointLight::attenuation(point);
}

float GPUPointLight::distanceToLight(vec3 point) {
    return PointLight::distanceToLight(point);
}
