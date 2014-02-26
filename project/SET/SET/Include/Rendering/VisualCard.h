#pragma once

#include "VisualObject.h"
#include "../Model/Card.h"
#include "../Animation/Animation.h"

class VisualGameScene;
class VisualCard : public VisualObject
{
public:
    enum class State
    {
        OnDesk, Choosed, Discarded
    };
private:
    CardRef Card_;
    ref<Animation> Animation_;
    VisualGameScene *Parent_;
    State State_;
public:
    VisualCard(const CardRef card, VisualGameScene *parent);

    ~VisualCard() { }

    bool GetIsChoosed() const { return CurrentState == State::Choosed; }
    
    virtual void OnMouseMove() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;
    virtual void OnMouseButtonDown() override;

    void Choosed();
    void CancelChoosed();
    void Discarded();
protected:
    virtual void RenderContent() override;
public:
    CardRef readwrite(Card);
    void SetCard(const CardRef card) { Card_ = card; }
    CardRef GetCard() { return Card_; }

    State readwrite(CurrentState);
    State GetCurrentState() const { return State_; }
    void SetCurrentState(State state) { State_ = state; }
public:
    static double HeightPerWidthRatio;
    static double DepthPerWidthRatio;
};
