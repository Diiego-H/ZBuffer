#pragma once

#include "GPUConnections/GPUConnectable.hh"
#include "VW/Lights/Light.hh"
using namespace std;

// Identificadors memòria GPU
struct gl_IDLight {
    GLuint type;
    GLuint Ia;
    GLuint Id;
    GLuint Is;
    GLuint pos;
    GLuint direction;
    GLuint angle;
    GLuint a;
    GLuint b;
    GLuint c;
};

class GPULight: public virtual Light, public GPUConnectable {

public:
    /*
     * Constructor de la classe Light.
     * param Ia: component ambient de la llum.
     * param Id: component difosa de la llum.
     * param Is: component especular de la llum.
     * */
    GPULight(vec3 Ia, vec3 Id, vec3 Is);
    GPULight();
    virtual ~GPULight() {};

    //Calcula el factor d'atenuacio de la llum al punt passat per parametre
    virtual float attenuation(vec3 point) override = 0;
    //Calcula el vector L amb origen el punt passat per parametre
    virtual vec3 vectorL(vec3 point) override = 0;
    //Calcula la distancia del punt a la llum
    virtual float distanceToLight(vec3 point) override = 0;

    virtual void toGPU(shared_ptr<QGLShaderProgram> p) override;
    virtual void bindMemory(gl_IDLight *id);
};

