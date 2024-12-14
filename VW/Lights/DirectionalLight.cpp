#include "DirectionalLight.hh"

DirectionalLight::DirectionalLight(vec3 direction, vec3 Ia, vec3 Id, vec3 Is): Light(Ia, Id, Is) {
    this->direction = normalize(direction);
}

vec3 DirectionalLight::getDirection() {
    return this->direction;
}

vec3 DirectionalLight::vectorL(vec3 point) {
    return -direction;
}

float DirectionalLight::attenuation(vec3 point) {
    // A llums direccionals no hi ha atenuació
    return 1.0f;
}

float DirectionalLight::distanceToLight(vec3 point) {
    return std::numeric_limits<float>::infinity();
}

void DirectionalLight::read (const QJsonObject &json)
{
    Light::read(json);

    if (json.contains("direction") && json["direction"].isArray()) {
        QJsonArray auxVec = json["direction"].toArray();
        direction[0] = auxVec[0].toDouble();
        direction[1] = auxVec[1].toDouble();
        direction[2] = auxVec[2].toDouble();
        direction = normalize(direction);
    }
}

//! [1]
void DirectionalLight::write(QJsonObject &json) const
{
    Light::write(json);
    QJsonArray auxArray;
    auxArray.append(direction[0]);auxArray.append(direction[1]);auxArray.append(direction[2]);
    json["direction"] = auxArray;
}
//! [1]

void DirectionalLight::print(int indentation) const
{
    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << "Directional Light" << "\n";
    QTextStream(stdout) << indent + 2 << "direction:\t" << direction[0] << ", "<< direction[1] << ", "<< direction[2] << "\n";
}

