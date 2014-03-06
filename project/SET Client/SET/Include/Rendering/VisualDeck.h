#pragma once

#include "VisualWidget.h"
#include "VisualCard.h"
#include "Operation.h"
#include "TextureManager.h"
#include "../Animation/VisualObjectAnimations.h"
class VisualDeck : public VisualWidget
{
    ref<Operation> Operation_;
    ref<Animation> HoverAnimation_;
    ref<Animation> BlinkAnimation_;
public:
    VisualDeck() : VisualWidget("default") { SetNormalEmission(Material_); }
    ref<::Operation> readwrite(Operation);
    ref<::Operation> GetOperation() { return Operation_; }
    void SetOperation(ref<::Operation> operation) { Operation_ = operation; }
    virtual void OnMouseButtonDown() override;

protected:
    virtual void RenderContent() override;
    virtual void OnMouseEnter() override;
    void Highlight(Material &m);
    virtual void OnMouseLeave() override;
    void SetNormalEmission(Material &m);
public:
    // invoke the blink animation.
    void Blink();
};