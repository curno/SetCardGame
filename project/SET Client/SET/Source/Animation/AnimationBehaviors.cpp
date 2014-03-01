#include "Include/stdafx.h"

#include "Include/Animation/AnimationBehaviors.h"

ref<AnimationBehavior> AnimationBehavior::LinearBehavior()
{
    static ref<AnimationBehavior> LinearBehavior_ = MakeGenericBehavior(
        [](double progress) -> double
        {
            return progress; // for linear behavior, return itself.
        }
    );
    return LinearBehavior_;
}

ref<AnimationBehavior> AnimationBehavior::SinBehavior()
{
    static ref<AnimationBehavior> SinBehavior_ = MakeGenericBehavior(
        [](double progress) -> double
    {
        return sin(progress * PI / 2); // for sin behavior, return sin(progress * PI / 2).
    }
    );
    return SinBehavior_;
}

ref<AnimationBehavior> AnimationBehavior::WiredBehavior()
{
    static ref<AnimationBehavior> WiredBehavior_ = MakeGenericBehavior(
        [](double progress) -> double
    {
        static const double factor = 0.2;
        static const double a = 1.0 / (1 - 2 * factor);
        static const double b = 1 - a;
        return a * progress * progress + b * progress;  // ax^2 + bx
    }
    );
    return WiredBehavior_;
}
