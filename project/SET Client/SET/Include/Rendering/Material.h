#pragma once

#include "stdafx.h"
// this class contains OpenGL material parameters.
// all parameters are public.
class Material
{
public:
    enum class Parameter
    {
        Ambient, Diffuse, Specular, Shininess, Emission
    };
    const char *Name;
    GLfloat Ambient[4];
    GLfloat Diffuse[4];
    GLfloat Specular[4];
    GLfloat Shininess[1];
    GLfloat Emission[4];

    void SetData(Parameter p, GLfloat *input);
    
public:
    static Material GetMaterial(const char *name);
};