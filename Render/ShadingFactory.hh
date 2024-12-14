#pragma once

#include "ShadingFactory.hh"
#include "ColorShading.hh"

class ShadingFactory
{
public:
    ShadingFactory() {};
    typedef enum  SHADING_TYPES{
        COLOR,
        NORMAL,
        BLINNPHONG
    } SHADING_TYPES;

    static ShadingFactory& getInstance() {
        static ShadingFactory instance;
        return instance;
    }

    shared_ptr<ShadingStrategy>   createShading(SHADING_TYPES t);
    SHADING_TYPES                 getShadingType(QString s);
    QString                       getNameType(SHADING_TYPES t);
    ShadingFactory::SHADING_TYPES getIndexType(shared_ptr<ShadingStrategy> m);
    
};

