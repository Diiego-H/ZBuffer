#pragma once

#include "Ray.hh"

class Hittable
{
public:
    Hittable(){};
    ~Hittable() {};


    // Funcio que retorna la intersecció més propera al t_min del raig. La intersecció
    // estarà entre t_min i t_max
     virtual bool hit (Ray& r, float tmin, float tmax) const = 0;


    // Mètode que retorna totes les interseccions que es troben al llarg del raig entre tmin i tmax
     virtual bool allHits(Ray& r, float tmin, float tmax) const = 0;

    // Metode que retorna un punt interior a una esfera de centre (0,0,0) i radi 1
    static vec3 RandomInSphere() {
        vec3 p;
        do {
            p = 2.0f*vec3(double(rand())/RAND_MAX, double(rand())/RAND_MAX,double(rand())/RAND_MAX) - vec3(1,1,1);
        } while (length(p) >=  1.0f);
        return p;
    }
};

