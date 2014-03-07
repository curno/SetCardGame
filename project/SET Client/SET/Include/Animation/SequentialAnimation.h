#pragma once

#include "Animation.h"
#include "AnimationManager.h"

// SequentialAnimation is a group of animations which are performed sequentially;
class SequentialAnimation : public Animation
{
    ::std::vector<ref<Animation>> Animations_;

public:
    virtual ~SequentialAnimation();
    virtual void Stop() override;
    void AddAnimation(ref<Animation> animation);

    // the sum of durations of the animations in this group is the SequentialAnimation's duration.
    virtual int GetDuration() override;

    virtual void OnAnimation(double progress) override;
};