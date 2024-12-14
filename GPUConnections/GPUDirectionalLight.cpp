#include "GPUConnections/GPUDirectionalLight.hh"

GPUDirectionalLight::GPUDirectionalLight(vec3 direction, vec3 Ia, vec3 Id, vec3 Is):DirectionalLight(direction, Ia, Id, Is) {
}

void GPUDirectionalLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO Pràctica 2: Fase 1: Pas 2: enviar les llums a la GPU
}

void GPUDirectionalLight::bindMemory(gl_IDLight *id) {
    #define DIRECTIONALLIGHT_TYPE 1

    // Bind Ia, Id, Is
    GPULight::bindMemory(id);

    // Bind variables específiques
    glUniform1i(id->type, DIRECTIONALLIGHT_TYPE);
    glUniform4fv(id->direction, 1, vec4(direction, 1));
}

vec3 GPUDirectionalLight::vectorL(vec3 point) {
    return DirectionalLight::vectorL(point);
}

float GPUDirectionalLight::attenuation(vec3 point) {
    return DirectionalLight::attenuation(point);
}

float GPUDirectionalLight::distanceToLight(vec3 point) {
    return DirectionalLight::distanceToLight(point);
}
