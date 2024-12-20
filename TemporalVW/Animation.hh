#pragma once

#include "VW/TG/TG.hh"
#include <vector>
#define MAXFRAMES 5

#include <memory>

using namespace std;

class Animation
{
public:
 int frameIni;
 int frameFinal;
 shared_ptr<TG> transf;

 Animation(): frameIni(0), frameFinal(5), transf(NULL) {}

 //  "operator =" per la classe  Animation
 Animation &operator =(const Animation &rhs) {
   frameIni = rhs.frameIni;
   frameFinal = rhs.frameFinal;
   transf = rhs.transf;
   return *this;
 }
};

class Animable {
public:
    Animable() {};
    ~Animable(){};

    void addAnimation(shared_ptr<Animation> anim);

    std::vector<shared_ptr<Animation>> animFrames;
    // update recorre la llista de frames per detectar quina animació aplicar.
    // crida a aplicaTG quan l'ha trobada
    void update(int nframe);
    // Obliga als objectes que tenen animacions implementar aquest mètode
    virtual void aplicaTG(shared_ptr<TG> tg) = 0;
};


