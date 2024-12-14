#pragma once

#include "GPUConnections/GPUConnectable.hh"
#include "VW/Materials/Material.hh"
using namespace std;

class GPUMaterial: public Material, public GPUConnectable {

public:
    GPUMaterial();
    GPUMaterial(vec3 d);
    GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess);
    GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess, float opacity);
    virtual ~GPUMaterial();

    virtual bool scatter(const Ray& r_in, int t, vec3& color, Ray & r_out) const {};
    virtual vec3 getDiffuse(vec2 point) const {return Kd;};


    virtual void toGPU(shared_ptr<QGLShaderProgram> p) override;
    virtual void draw();

    struct gl_IdMaterial {
        GLuint ka;
        GLuint kd;
        GLuint kdToon[4];
        GLuint ks;
        GLuint shininess;
        GLuint opacity;
    };

    gl_IdMaterial gl_id_material;

};

