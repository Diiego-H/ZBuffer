#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.hh"


class SpotLight: public virtual Light {
public:
    SpotLight() {};
    /*
     * Constructor de la classe SpotLight.
     * param posicio: posició de la llum.
     * param direction: eix de la llum.
     * param angle: obertura màxima (respecte l'eix) a il·luminar
     * param Ia: component ambient de la llum.
     * param Id: component difosa de la llum.
     * param Is: component especular de la llum.
     * param a: coeficient a de l'atenuacio.
     * param b: coeficient b de l'atenuacio.
     * param c: coeficient c de l'atenuacio.
     * */
    SpotLight(vec3 posicio, vec3 direction, float angle, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c);
    virtual ~SpotLight() {}
    vec3 getPos();
    vec3 getDirection();
    float getAngle();
    virtual vec3 vectorL(vec3 point) override;
    virtual float attenuation(vec3 point) override;
    virtual float distanceToLight(vec3 point) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;
protected:
    vec3 pos;
    vec3 direction;
    float angle;
    float a; // atenuacio: terme constant
    float b; // atenuacio: terme lineal
    float c; // atenuacio: terme quadratic
};

#endif // SPOTLIGHT_H
