#pragma once

#include "Animation.h"

// SequentialAnimation is a group of animations which are performed sequentially;
class SequentialAnimation : public Animation
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

    // the sum of durations of the animations in this group is the SequentialAnimation's duration.
    virtual int GetDuration() override
    {
        int retval = 0;
        for each (ref<Animation> animation in Animations_)
            retval += animation->Duration;
        return retval;
    }

    virtual double OnAnimationStep(const double ratio) override
    {
        __super::OnAnimationStep(ratio);
        int current_step = Duration * ratio;

        // perform current animation.
        int current = 0;
        for (auto i = Animations_.begin(); i != Animations_.end(); ++i)
        {
            int old_current = current;
            current += (*i)->Duration;
            if (current >= current_step)
            {
                (*i)->OnAnimationStep(static_cast<double>(current_step - old_current) / (*i)->Duration);
                break;
            }

        }

        return ratio; // no adjustment
    }
};