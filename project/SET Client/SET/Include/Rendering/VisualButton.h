#pragma once

#include "VisualWidget.h"
#include "Operation.h"
#include "TextureManager.h"

class VisualButton : public VisualWidget
{
    ref<Operation> Operation_;
    TextureManager::TextureName TextureName_;

public:
    VisualButton(ref<Operation> operation);
protected:

    virtual void RenderContent();

public:
    virtual void OnMouseButtonDown();

    virtual void PrepareRendering();

    void SetTextureName(TextureManager::TextureName texture) { TextureName_ = texture; }

    static const double HeightPerWidthRatio;
    static const double DepthPerWidthRatio;
private:
    // display parameters.
    static const double CornerRatioX;
    static const double CornerRatioY;

};