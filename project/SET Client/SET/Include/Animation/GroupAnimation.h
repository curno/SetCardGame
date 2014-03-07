#pragma once

#include "Animation.h"
#include "AnimationManager.h"

// GroupAnimation is a group of animations which are performed in parallel.
class GroupAnimation : public Animation
{
    ::std::vector<ref<Animation>> Animations_;
public:
    ~GroupAnimation();
    virtual void Stop() override;
    void AddAnimation(ref<Animation> animation);

    // the longest duration of the animations in this group is the GroupAnimation's duration.
    virtual int GetDuration() override;

    virtual void OnAnimation(double progress) override;
};