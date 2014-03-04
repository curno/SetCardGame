#pragma once

#include "VisualObject.h"

class VisualWidget : public VisualObject
{
public:
    VisualWidget(const char *meterial) : VisualObject(meterial), EmissionHighlight_(0.7f), EmissionNormal_(0.3f), EmissionDisabled_(0.0f) { UpdateMaterialEmission(); }
    

    void OnMouseEnter() override
    {
        __super::OnMouseEnter();
        if (Enabled)
            SetMaterialEmission(EmissionHighlight_);
    }

    void OnMouseLeave() override
    {
        __super::OnMouseLeave();
        if (Enabled)
            SetMaterialEmission(EmissionNormal_);
    }

    void SetMaterialEmission(GLfloat emission_value)
    {
        GLfloat emission[] = { emission_value, emission_value, emission_value, 1.0f };
        Material_.SetData(Material::Parameter::Emission, emission);
    }

    void SetEnabled(bool enable) override
    {
        __super::SetEnabled(enable);
        UpdateMaterialEmission();
    }

protected:
    void UpdateMaterialEmission()
    {
        if (Enabled)
            SetMaterialEmission(EmissionNormal_);
        else
            SetMaterialEmission(EmissionDisabled_);
    }

protected:
    GLfloat EmissionHighlight_;
    GLfloat EmissionNormal_;
    GLfloat EmissionDisabled_;
};