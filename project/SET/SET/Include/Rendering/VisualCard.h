#pragma once

#include "VisualObject.h"
#include "../Model/Card.h"
#include "../Animation/Animation.h"

class VisualCard : public VisualObject
{
    bool Choosed_;
    CardRef Card_;
    ref<Animation> Animation_;
public:
    VisualCard(const CardRef card);

    ~VisualCard() { }

    virtual void OnMouseMove() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;
    virtual void OnMouseButtonDown() override;
protected:
    virtual void RenderContent() override;

    void SetCard(const CardRef card) { Card_ = card; }

public:
    static double HeightPerWidthRatio;
    static double DepthPerWidthRatio;
};
