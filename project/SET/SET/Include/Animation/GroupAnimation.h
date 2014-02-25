#pragma once

#include "Animation.h"

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
    }

    // the longest duration of the animations in this group is the GroupAnimation's duration.
    virtual int GetDuration() override
    {
        int retval = 0;
        for each (ref<Animation> animation in Animations_)
            retval = max(retval, animation->Duration);
        return retval;
    }

    virtual void OnAnimationStep(const double ratio) override
    {
        __super::OnAnimationStep(ratio);
        int current_step = Duration * ratio;

        // perform each animation in parallel with proper ratio.
        for each (ref<Animation> animation in Animations_)
            animation->OnAnimationStep(static_cast<double>(current_step) / animation->Duration);
    }

};