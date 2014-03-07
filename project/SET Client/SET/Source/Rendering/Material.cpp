#include "Include/stdafx.h"

#include "Include/Rendering/Material.h"

Material Material::GetMaterial(const char *name)
{
    // build in materials, from http://devernay.free.fr/cours/opengl/materials.html
    static const  Material BuildInMaterial[] = {
        { "default", 0.3f, 0.3f, 0.3f, 1.0f, 0.8f, 0.8f, 0.8f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "emerald", 0.0215f, 0.1745f, 0.0215f, 1.0f, 0.07568f, 0.61424f, 0.07568f, 1.0f, 0.633f, 0.727811f, 0.633f, 1.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "jade", 0.135f, 0.2225f, 0.1575f, 1.0f, 0.54f, 0.89f, 0.63f, 1.0f, 0.316228f, 0.316228f, 0.316228f, 1.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "obsidian", 0.05375f, 0.05f, 0.06625f, 1.0f, 0.18275f, 0.17f, 0.22525f, 1.0f, 0.332741f, 0.328634f, 0.346435f, 1.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "pearl", 0.25f, 0.20725f, 0.20725f, 1.0f, 1.0f, 0.829f, 0.829f, 1.0f, 0.296648f, 0.296648f, 0.296648f, 1.0f, 0.088f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "ruby", 0.1745f, 0.01175f, 0.01175f, 1.0f, 0.61424f, 0.04136f, 0.04136f, 1.0f, 0.727811f, 0.626959f, 0.626959f, 1.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "turquoise", 0.1f, 0.18725f, 0.1745f, 1.0f, 0.396f, 0.74151f, 0.69102f, 1.0f, 0.297254f, 0.30829f, 0.306678f, 1.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "brass", 0.329412f, 0.223529f, 0.027451f, 1.0f, 0.780392f, 0.568627f, 0.113725f, 1.0f, 0.992157f, 0.941176f, 0.807843f, 1.0f, 0.21794872f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "bronze", 0.2125f, 0.1275f, 0.054f, 1.0f, 0.714f, 0.4284f, 0.18144f, 1.0f, 0.393548f, 0.271906f, 0.166721f, 1.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "chrome", 0.25f, 0.25f, 0.25f, 1.0f, 0.4f, 0.4f, 0.4f, 1.0f, 0.774597f, 0.774597f, 0.774597f, 1.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "copper", 0.19125f, 0.0735f, 0.0225f, 1.0f, 0.7038f, 0.27048f, 0.0828f, 1.0f, 0.256777f, 0.137622f, 0.086014f, 1.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "gold", 0.24725f, 0.1995f, 0.0745f, 1.0f, 0.75164f, 0.60648f, 0.22648f, 1.0f, 0.628281f, 0.555802f, 0.366065f, 1.0f, 0.4f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "silver", 0.19225f, 0.19225f, 0.19225f, 1.0f, 0.50754f, 0.50754f, 0.50754f, 1.0f, 0.508273f, 0.508273f, 0.508273f, 1.0f, 0.4f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "black plastic", 0.0f, 0.0f, 0.0f, 1.0f, 0.01f, 0.01f, 0.01f, 1.0f, 0.50f, 0.50f, 0.50f, 1.0f, .25f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "cyan plastic", 0.0f, 0.1f, 0.06f, 1.0f, 0.0f, 0.50980392f, 0.50980392f, 1.0f, 0.50196078f, 0.50196078f, 0.50196078f, 1.0f, .25f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "green plastic", 0.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.35f, 0.1f, 1.0f, 0.45f, 0.55f, 0.45f, 1.0f, .25f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "red plastic", 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.7f, 0.6f, 0.6f, 1.0f, .25f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "white plastic", 0.0f, 0.0f, 0.0f, 1.0f, 0.55f, 0.55f, 0.55f, 1.0f, 0.70f, 0.70f, 0.70f, 1.0f, .25f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "yellow plastic", 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.60f, 0.60f, 0.50f, 1.0f, .25f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "black rubber", 0.02f, 0.02f, 0.02f, 1.0f, 0.01f, 0.01f, 0.01f, 1.0f, 0.4f, 0.4f, 0.4f, 1.0f, .078125f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "cyan rubber", 0.0f, 0.05f, 0.05f, 1.0f, 0.4f, 0.5f, 0.5f, 1.0f, 0.04f, 0.7f, 0.7f, 1.0f, .078125f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "green rubber", 0.0f, 0.05f, 0.0f, 1.0f, 0.4f, 0.5f, 0.4f, 1.0f, 0.04f, 0.7f, 0.04f, 1.0f, .078125f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "red rubber", 0.05f, 0.0f, 0.0f, 1.0f, 0.5f, 0.4f, 0.4f, 1.0f, 0.7f, 0.04f, 0.04f, 1.0f, .078125f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "white rubber", 0.05f, 0.05f, 0.05f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.7f, 0.7f, 0.7f, 1.0f, .078125f, 0.0f, 0.0f, 0.0f, 0.0f },
        { "yellow rubber", 0.05f, 0.05f, 0.0f, 1.0f, 0.5f, 0.5f, 0.4f, 1.0f, 0.7f, 0.7f, 0.04f, 1.0f, .078125f, 0.0f, 0.0f, 0.0f, 0.0f }
    };

    for (int i = 0; i < sizeof(BuildInMaterial) / sizeof(BuildInMaterial[0]); ++i)
    {
        if (strcmp(BuildInMaterial[i].Name, name) == 0)
            return BuildInMaterial[i];
    }
    return BuildInMaterial[0];
}

void Material::SetData(Parameter p, GLfloat *input)
{
    GLfloat *data;
    int count;
    switch (p)
    {
    case Parameter::Ambient:
        data = Ambient;
        count = 4;
        break;
    case Parameter::Diffuse:
        data = Diffuse;
        count = 4;
        break;
    case Parameter::Emission:
        data = Emission;
        count = 4;
        break;
    case Parameter::Specular:
        data = Specular;
        count = 4;
        break;
    case Parameter::Shininess:
        data = Shininess;
        count = 1;
        break;
    }
    memcpy(data, input, sizeof(GLfloat)* count);
}
