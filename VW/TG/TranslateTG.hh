#pragma once

#include "TG.hh"

class TranslateTG : public TG
{
public:
    vec3 traslation;
    TranslateTG(vec3 trasl);
    virtual ~TranslateTG();
};

