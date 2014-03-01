#pragma once

#include "VisualObject.h"
#include "Operation.h"
#include "TextureManager.h"

class VisualButton : public VisualObject
{
    ref<Operation> Operation_;
    TextureManager::TextureName TextureName_;
public:
    VisualButton(ref<Operation> operation) : Operation_(operation), TextureName_(0)
    {
        Material_ = Material::GetMaterial("ruby");
        GLfloat emission[] = { 0.3f, 0.3f, 0.3f, 1.0f };
        Material_.SetData(Material::Parameter::Emission, emission);
    }
protected:

    virtual void RenderContent();

public:
    virtual void OnMouseButtonDown()
    {
        __super::OnMouseButtonDown();
        if (!Enabled)
            return;
        if (Operation_ != nullptr)
            (*Operation_)();
    }

    virtual void PrepareRendering();

    virtual void OnMouseEnter();

    virtual void OnMouseLeave();

    virtual void OnMouseMove();

    void SetTextureName(TextureManager::TextureName texture) { TextureName_ = texture; }

    static const double HeightPerWidthRatio;
    static const double DepthPerWidthRatio;
private:
    static const double CornerRatioX;
    static const double CornerRatioY;
};