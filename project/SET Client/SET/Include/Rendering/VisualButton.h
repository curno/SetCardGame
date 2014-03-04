#pragma once

#include "VisualWidget.h"
#include "Operation.h"
#include "TextureManager.h"

class VisualButton : public VisualWidget
{
    ref<Operation> Operation_;
    TextureManager::TextureName TextureName_;

public:
    VisualButton(ref<Operation> operation) : VisualWidget("ruby"), Operation_(operation), TextureName_(0)
    {
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

    void SetTextureName(TextureManager::TextureName texture) { TextureName_ = texture; }

    static const double HeightPerWidthRatio;
    static const double DepthPerWidthRatio;
private:
    static const double CornerRatioX;
    static const double CornerRatioY;

   
};