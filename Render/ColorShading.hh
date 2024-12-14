#pragma once

#include "ShadingStrategy.hh"

class ColorShading: public ShadingStrategy
{
public:
    ColorShading() {};
    vec3 shading(shared_ptr<Scene> scene, vector<shared_ptr<HitRecord>> info, vec3 lookFrom, bool shadow) override;
    ~ColorShading(){};
};


