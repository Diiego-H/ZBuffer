#include "GPUConnections/GPUSpotLight.hh"

GPUSpotLight::GPUSpotLight(vec3 posicio, vec3 direction, float angle, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c): SpotLight(posicio, direction, angle, Ia, Id, Is, a, b, c) {
}

void GPUSpotLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO Pràctica 2: Fase 1: Pas 2: enviar les llums a la GPU
}

void GPUSpotLight::bindMemory(gl_IDLight *id) {
    #define SPOTLIGHT_TYPE 2

    // Bind Ia, Id, Is
    GPULight::bindMemory(id);

    // Bind variables específiques
    glUniform1i(id->type, SPOTLIGHT_TYPE);
    glUniform4fv(id->pos, 1, vec4(pos, 1));
    glUniform4fv(id->direction, 1, vec4(direction, 1));
    glUniform1f(id->angle, angle);
    glUniform1f(id->a, a);
    glUniform1f(id->b, b);
    glUniform1f(id->c, c);
}

vec3 GPUSpotLight::vectorL(vec3 point) {
    return SpotLight::vectorL(point);
}

float GPUSpotLight::attenuation(vec3 point) {
    return SpotLight::attenuation(point);
}

float GPUSpotLight::distanceToLight(vec3 point) {
    return SpotLight::distanceToLight(point);
}
