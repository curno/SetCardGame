#pragma once

#include "Animation.h"
#include "AnimationManager.h"

// SequentialAnimation is a group of animations which are performed sequentially;
class SequentialAnimation : public Animation
{
    ::std::vector<ref<Animation>> Animations_;

public:
    virtual ~SequentialAnimation() { Stop(); }
    virtual void Stop() override
    {
        for each (ref<Animation> animation in Animations_)
            animation->Stop();
        __super::Stop();
    }
    void AddAnimation(ref<Animation> animation)
    {
        // already in the group.
        if (::std::find(Animations_.begin(), Animations_.end(), animation) != Animations_.end())
            return;
        Animations_.push_back(animation);

        // remove from the AnimationManager, because animation is not independent anymore
        AnimationManager::Instance().RemoveAnimation(animation.get());
    }

    // the sum of durations of the animations in this group is the SequentialAnimation's duration.
    virtual int GetDuration() override
    {
        int retval = 0;
        for each (ref<Animation> animation in Animations_)
            retval += animation->Duration;
        return retval;
    }

    virtual void OnAnimation(double progress) override
    {
        __super::OnRatio(progress);
        double current_step = Duration * progress;

        // perform current animation.
        int current = 0;
        for (auto i = Animations_.begin(); i != Animations_.end(); ++i)
        {
            
            int old_current = current;
            if (current <= current_step)
            {
                if (!(*i)->IsStopped)
                {
                    double sub_progress = (current_step - old_current) / (*i)->Duration;
                    (*i)->OnAnimation(sub_progress);
                    if (!Animation::ValidProgress(sub_progress))
                        (*i)->Stop();
                }
            }
            else
                break;
            current += (*i)->Duration;

        }
    }
};