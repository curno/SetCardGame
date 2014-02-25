#pragma once

#include "Animation.h"

// This is a decorate class of a animation which will perform the animation repeatedly.
// Design Pattern: Decorator

class LoopAnimation : public Animation
{
    ref<Animation> Target_;
public:
    LoopAnimation(ref<Animation> target) : Target_(target) { }

    virtual void OnAnimationStep(const double ratio) override
    {
        // pass to target
        Target_->OnAnimationStep(ratio);
    }

    virtual double GetCurrentProcess() override
    {
        //elapsed time
        Watch_.Stop();
        auto passed = Watch_.ElapsedMilliseconds;
        Watch_.Start();

        // get duration
        int duration = Duration;

        return static_cast<double>(passed % duration) / duration; // looping
    }

    virtual int GetDuration() override { return Target_->Duration;  }

};