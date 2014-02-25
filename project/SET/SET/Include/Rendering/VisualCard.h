#pragma once

#include "VisualObject.h"
#include "../Model/Card.h"
#include "../Animation/Animation.h"

class VisualGameScene;
class VisualCard : public VisualObject
{
    bool Choosed_;
    CardRef Card_;
    ref<Animation> Animation_;
    VisualGameScene *Parent_;
public:
    VisualCard(const CardRef card, VisualGameScene *parent);

    ~VisualCard() { }

    bool GetIsChoosed() const { return Choosed_; }
    virtual void OnMouseMove() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;
    virtual void OnMouseButtonDown() override;

    void Choosed();

    void CancelChoosed();

protected:
    virtual void RenderContent() override;
public:
    CardRef readwrite(Card);
    void SetCard(const CardRef card) { Card_ = card; }
    CardRef GetCard() { return Card_; }
public:
    static double HeightPerWidthRatio;
    static double DepthPerWidthRatio;
};
