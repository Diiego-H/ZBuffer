#pragma once

#include <memory>
#include "library/Common.h"


using namespace Common;
using namespace std;

class Material;


class HitRecord
{
public:
    float                t;         // t del raig on hi ha intersecció
    vec3                 p;         // punt del raig on hi ha la intersecció
    vec3                 normal;    // normal en el punt d'intersecció
    shared_ptr<Material> mat;   // material de l'objecte que s'ha intersectat
    vec2                 uv;        // punt 2D per la projeccio de la textura

    HitRecord():
        t(std::numeric_limits<float>::infinity()),
        p(0.0f),
        normal(0.0f),
        uv(0.0f) {}

    //  "operator =" per la classe  HitRecord
    HitRecord &operator =(const HitRecord &rhs) {
      p = rhs.p;
      mat = rhs.mat;
      normal = rhs.normal;
      t = rhs.t;
      uv = rhs.uv;
      return *this;
    }
};
