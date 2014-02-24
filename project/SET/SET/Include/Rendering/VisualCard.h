#pragma once

#include "VisualObject.h"
#include "../Model/Card.h"
#include "Res/resource.h"
#include "Material.h"

class VisualCard : public VisualObject
{
    static const int DefaultWidth = 100;
    static const int DefaultHeight = 200;
    CardRef Card_;

    Material Material_;
public:
    VisualCard(const CardRef card);

    

    ~VisualCard() { }

    virtual void OnMouseMove() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;
protected:
    virtual void RenderContent() override;
    virtual void PrepareRendering() override;
    void SetCard(const CardRef card) { Card_ = card; }
};
