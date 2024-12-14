#pragma once

#include "Scene.hh"

class ShadingStrategy {
 public:
    // TODO: Fase 2: Canviar el mètode per passar les llums per calcular el shading
    virtual vec3 shading(shared_ptr<Scene> scene, vector<shared_ptr<HitRecord>> info, vec3 lookFrom, bool shadow) {
        return vec3(0.0, 0.0, 0.0);
    };
    

    // TUTORIAL 2: Calcula si el punt "point" és a l'ombra segons si el flag està activat o no
    // float computeShadow(shared_ptr<Light> light, vec3 point);

    virtual ~ShadingStrategy() {};
};