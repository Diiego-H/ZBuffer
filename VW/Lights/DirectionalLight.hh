#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.hh"


class DirectionalLight: public virtual Light {
public:
    DirectionalLight() {};
    /*
     * Constructor de la classe DirectionalLight.
     * param direction: direcció de la llum.
     * param Ia: component ambient de la llum.
     * param Id: component difosa de la llum.
     * param Is: component especular de la llum.
     * */
    DirectionalLight(vec3 direction, vec3 Ia, vec3 Id, vec3 Is);
    virtual ~DirectionalLight() {}
    vec3 getDirection();
    virtual vec3 vectorL(vec3 point) override;
    virtual float attenuation(vec3 point) override;
    virtual float distanceToLight(vec3 point) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;
protected:
    vec3 direction;
};

#endif // DIRECTIONALLIGHT_H
