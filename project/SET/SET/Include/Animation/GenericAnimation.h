#pragma once

#include "Animation.h"

struct AnimationCallable abstract
{
    double operator ()(double ratio);
};

template <typename AnimationCallable>
class GenericAnimation : public Animation
{
    AnimationCallable Animation_;
    int Duration_;
public:
    GenericAnimation(int duration, AnimationCallable animate) : Duration_(duration), Animation_(animate) { }

    virtual int GetDuration() override { return Duration_; }
    virtual void OnRatio(const double ratio) override { Animation_(ratio); }
};

template <typename AnimationCallable>
ref<GenericAnimation<AnimationCallable>> MakeGenericAnimation(int duration, AnimationCallable callable)
{
    return ::std::make_shared<GenericAnimation<AnimationCallable>>(duration, callable);
}