#pragma once

#include "VisualObject.h"
#include "../Model/Card.h"
#include "Res/resource.h"
#include "Material.h"

class VisualCard : public VisualObject
{
    CardRef Card_;

public:
    VisualCard(const CardRef card);

    ~VisualCard() { }

    virtual void OnMouseMove() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;
protected:
    virtual void RenderContent() override;

    void SetCard(const CardRef card) { Card_ = card; }

public:
    static double HeightPerWidthRatio;
    static double DepthPerWidthRatio;
};
