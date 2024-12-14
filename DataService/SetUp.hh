#pragma once

#include "DataService/Serializable.hh"

#include <QFile>

#include "library/Common.h"
#include "VW/Lights/LightFactory.hh"

#include "Render/ShadingFactory.hh"
#include "Render/ShadingStrategy.hh"
#include "Render/ColorShading.hh"

class SetUp : public Serializable
{
public:
    SetUp();

    QString                         getOutputFile();
    vec3                            getGlobalLight();
    std::vector<shared_ptr<Light>>  getLights();
    shared_ptr<ShadingStrategy>     getShadingStrategy() {return shade;}
    bool                            getBackground();
    vec3                            getTopBackground();
    vec3                            getDownBackground();
    int                             getMAXDEPTH();
    int                             getSamples();
    bool                            getSoftShadows() {return softShadows;}
    bool                            getNHints() {return nHints;}
    bool                            getShadows() {return shadows;}
    bool                            getBoundingVolumes() {return boundingVolumes;}


    void setOutpuFile(QString name);
    void setGlobalLight(vec3 globalLight);
    void setLights(std::vector<shared_ptr<Light>> lights);
    void setShadingStrategy(shared_ptr<ShadingStrategy> s) {shade = s;}
    void setBackground(bool back);
    void setTopBackground(vec3 color);
    void setDownBackground(vec3 color);
    void setSamples(int s);
    void setSoftShadows(bool b);
    void setNHints(bool b);
    void setShadows(bool b);
    void setBoundingVolumes(bool b);
    void setMAXDEPTH(int depth);

    virtual void read (const QJsonObject &json);
    virtual void write (QJsonObject &json) const;
    virtual void print (int indentation) const;

    bool load( QString nameFile);
    bool save( QString nameFile) const;

    virtual ~SetUp() {};

protected:
    // Llum global
    vec3 globalLight;

private:
    QString outputFile;

    // Shading utilitzat
    shared_ptr<ShadingStrategy> shade;

    // TUTORIAL 2: Afegir llums a l'escena
    // Vector de llums de l'escena
    std::vector<shared_ptr<Light>> lights;

    // Conté si s'ha de tenir en compte el background o no en els rajos secundaris en
    // cas de no fer hit amb l'escena
    bool background = false;
    // colors top i bottom de  fons
    vec3 topBackground;
    vec3 downBackground;

    // Boolea que indica si es vol tenir només un hint en el raig o més d'un
    bool nHints;
    // màxim nombre de rajos recursius incloent el primari
    int   MAXDEPTH;

    // number of samples per pixels
    int   numSamples;

    // flags per activar funcionalitats del getcolorPixel de RayTracing
     bool shadows;
     bool softShadows;
     bool boundingVolumes;

};

