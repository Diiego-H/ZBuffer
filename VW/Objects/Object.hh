#pragma once

#include "Rays/Hittable.hh"
#include "TemporalVW/Animation.hh"

#include "VW/Materials/MaterialFactory.hh"


using namespace std;
// Es la classe pare de tots els objectes que s'han de visualitzar.
// Es fa una classe Objectes que hereda de Hitable i ha d'implementar el metode intersection

class Object: public Hittable, public Animable, public Serializable {
  public:
    Object();
    virtual ~Object() {};

  
    // Metodes a implementar en les classes filles: son  metodes abstractes
    virtual bool hit(Ray& r, float tmin, float tmax) const override = 0;
    virtual bool allHits(Ray& r, float tmin, float tmax) const override = 0;

    virtual void aplicaTG(shared_ptr<TG>) override = 0 ;

    // OPCIONAL: Mètode que retorna totes les interseccions de l'objecte
    //    virtual bool allHits(const Ray& r, vector<shared_ptr<HitInfo> infos) const = 0;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    void     setMaterial(shared_ptr<Material> m);

    shared_ptr<Material> getMaterial();
    QString name;

protected:
    shared_ptr<Material> material;   // Material de l'objecte
    // Valor pel mapping indirecte de textures, per crear una esfera al voltant de l'objecte
    float radi_max;
    GLuint radi_a_enviar;

};

