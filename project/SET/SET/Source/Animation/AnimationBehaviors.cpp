#include "Include/stdafx.h"

#include "Include/Animation/AnimationBehaviors.h"

ref<AnimationBehavior> AnimationBehavior::LinearBehavior()
{
    static ref<AnimationBehavior> LinearBehavior_ = MakeGenericAnimation(
        [](double progress) -> double
        {
            return progress; // for linear behavior, return itself.
        }
    );
    return LinearBehavior_;
}

ref<AnimationBehavior> AnimationBehavior::SinBehavior()
{
    static ref<AnimationBehavior> SinBehavior_ = MakeGenericAnimation(
        [](double progress) -> double
    {
        return sin(progress * PI / 2); // for sin behavior, return sin(progress * PI / 2).
    }
    );
    return SinBehavior_;
}

ref<AnimationBehavior> AnimationBehavior::WiredBehavior()
{
    static ref<AnimationBehavior> WiredBehavior_ = MakeGenericAnimation(
        [](double progress) -> double
    {
        return sin(progress * 8.3); 
    }
    );
    return WiredBehavior_;
}
