#include "GPUConnections/GPULight.hh"

GPULight::GPULight(vec3 Ia, vec3 Id, vec3 Is): Light(Ia, Id, Is){
}

GPULight::GPULight() {
}

void GPULight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO Pràctica 2: Fase 1: enviar les propietats de Ia, Id i Is a la GPU
}

void GPULight::bindMemory(gl_IDLight *id) {
    glUniform3fv(id->Ia, 1, Ia);
    glUniform3fv(id->Id, 1, Id);
    glUniform3fv(id->Is, 1, Is);
}