#pragma once

#include "Animation.h"
#include "AnimationManager.h"

// GroupAnimation is a group of animations which are performed in parallel.
class GroupAnimation : public Animation
{
    ::std::vector<ref<Animation>> Animations_;
public:
    void AddAnimation(ref<Animation> animation)
    {
        // already in the group.
        if (::std::find(Animations_.begin(), Animations_.end(), animation) != Animations_.end())
            return;
        Animations_.push_back(animation);

        // remove from the AnimationManager, because animation is not independent anymore
        AnimationManager::Instance().RemoveAnimation(animation.get());
    }

    // the longest duration of the animations in this group is the GroupAnimation's duration.
    virtual int GetDuration() override
    {
        int retval = 0;
        for each (ref<Animation> animation in Animations_)
            retval = max(retval, animation->Duration);
        return retval;
    }

    virtual void OnAnimation(double progress) override
    {
        __super::OnAnimation(progress);
        int current_step = Duration * progress;

        // perform each animation in parallel with proper ratio.
        for each (ref<Animation> animation in Animations_)
            animation->OnAnimation(static_cast<double>(current_step) / animation->Duration);
    }

};