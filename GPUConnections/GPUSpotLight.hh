#pragma once

#include "GPUConnections/GPULight.hh"
#include "VW/Lights/SpotLight.hh"

class GPUSpotLight: public GPULight, public SpotLight {
public:
    GPUSpotLight() {};
    /*
     * Constructor de la classe SpotLight.
     * param posicio: posició de la llum.
     * param direction: direcció de la llum.
     * param angle: obertura màxima (respecte l'eix) a il·luminar
     * param Ia: component ambient de la llum.
     * param Id: component difosa de la llum.
     * param Is: component especular de la llum.
     * param a: coeficient a de l'atenuacio.
     * param b: coeficient b de l'atenuacio.
     * param c: coeficient c de l'atenuacio.
     * */
    GPUSpotLight(vec3 posicio, vec3 direction, float angle, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c);
    virtual ~GPUSpotLight() {}

    virtual void toGPU(shared_ptr<QGLShaderProgram> p) override;
    virtual void bindMemory(gl_IDLight *id) override;
    virtual vec3 vectorL(vec3 point) override;
    virtual float attenuation(vec3 point) override;
    virtual float distanceToLight(vec3 point) override;
};

