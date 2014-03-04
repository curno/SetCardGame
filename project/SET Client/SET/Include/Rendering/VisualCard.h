#pragma once

#include "VisualWidget.h"
#include "../Model/Card.h"
#include "../Animation/Animation.h"

class VisualGameScene;
class VisualCard;
typedef ref<VisualCard> VisualCardRef;
class VisualCard : public VisualWidget
{
public:
    enum class State
    {
        OnDesk, Choosed, Discarded
    };
private:
    CardRef Card_; // the internal card
    VisualGameScene *Parent_; // the parent scene, must not be null.
    State State_; // current state

    ref<Animation> ChoosedAnimation_; // ChoosedAnimation is using build-in transformation of the VisualCard.

    Transformation ShakeAnimationTransformation_; 
    ref<Animation> ShakeAnimation_; // ShakeAnimation is using ShakeAnimationTransformation_;
public:
    VisualCard(const CardRef card, VisualGameScene *parent);

    ~VisualCard() { }

    bool GetIsChoosed() const { return CurrentState == State::Choosed; }
    
    virtual void OnMouseMove() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;
    virtual void OnMouseButtonDown() override;

    void Choosed();
    void ChoosedAnimate();

    void CancelChoosed();
    void CancelChoosedAnimate();

    void Discarded();
    void Shake();
protected:
    virtual void RenderContent() override;
    virtual void PrepareRendering() override;
public:
    CardRef readonly(Card);
    CardRef GetCard() { return Card_; }

    State readwrite(CurrentState);
    State GetCurrentState() const { return State_; }
    void SetCurrentState(State state) { State_ = state; }

private:
    void RenderFrontRectangle(GLdouble p0x, GLdouble p0y, GLdouble p1x, GLdouble p1y);
public:
    static double HeightPerWidthRatio;
    static double DepthPerWidthRatio;
    static double SymbolRatio;
};
