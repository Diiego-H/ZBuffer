#include "GPUMaterial.hh"

GPUMaterial::GPUMaterial() : Material() {

}

GPUMaterial::GPUMaterial(vec3 d) : Material(d) {

}
GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess) : Material(a, d, s, shininess) {

}
GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess, float opacity) : Material(a, d, s, shininess, opacity) {

}

GPUMaterial::~GPUMaterial() {

}

void GPUMaterial::toGPU(shared_ptr<QGLShaderProgram> p) {

    program = p;
    // set up material
    gl_id_material.ka = program->uniformLocation("material.ka");
    gl_id_material.kd = program->uniformLocation("material.kd");
    for (int i=0; i<NUM_TOON; i++) {
        gl_id_material.kdToon[i] = program->uniformLocation(QString("material.kdToon[%1]").arg(i));
    }
    gl_id_material.ks = program->uniformLocation("material.ks");
    gl_id_material.shininess = program->uniformLocation("material.shininess");
    gl_id_material.opacity = program->uniformLocation("material.opacity");

    glUniform3fv(gl_id_material.ka, 1, Ka);
    glUniform3fv(gl_id_material.kd, 1, Kd);
    for (int i=0; i<NUM_TOON; i++) {
        glUniform3fv(gl_id_material.kdToon[i], 1, KdToon[i]);
    }
    glUniform3fv(gl_id_material.ks, 1, Ks);
    glUniform1f(gl_id_material.shininess, shininess);
    glUniform1f(gl_id_material.opacity, opacity);
}

void GPUMaterial::draw() {
    glUniform3fv(gl_id_material.ka, 1, Ka);
    glUniform3fv(gl_id_material.kd, 1, Kd);
    for (int i=0; i<NUM_TOON; i++) {
        glUniform3fv(gl_id_material.kdToon[i], 1, KdToon[i]);
    }
    glUniform3fv(gl_id_material.ks, 1, Ks);
    glUniform1f(gl_id_material.shininess, shininess);
    glUniform1f(gl_id_material.opacity, opacity);
}