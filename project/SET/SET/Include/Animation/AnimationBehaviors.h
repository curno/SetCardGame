#pragma once

#include "stdafx.h"
// this class is designed to describe the behavior of a animation.
// for example, a linear behavior is a behavior that perform animation linearly.
// a Sin behavior will perform the animation faster at first and slower later, following the curve of a sin curve
// it is defined by a member function which transform the input process (between 0.0 to 1.0) to a animation ratio.
class AnimationBehavior abstract
{
public:
    virtual double GetRatio(double process) = 0;

public:
    // Design Pattern: singleton
    static ref<AnimationBehavior> LinearBehavior();
    static ref<AnimationBehavior> SinBehavior();

    static ref<AnimationBehavior> WiredBehavior(); // for test and fun.:)
};



// this is the generic class of a AnimationBehavior which takes a callable object as its behavior.
// Design Pattern: strategy
// the callable object is the strategy
template <typename Callable>
class GenericAnimationBehavior : public AnimationBehavior
{
    Callable Behavior_;
public:
    GenericAnimationBehavior(Callable callable) : Behavior_(callable) { }
    virtual double GetRatio(double process) override { return Behavior_(process); /* call strategy object */ }
};

// the convenient creation method for GenericAnimationBehavior<...>
template <typename Callable>
ref<GenericAnimationBehavior<Callable>> MakeGenericBehavior(Callable callable) 
{ 
    return ::std::make_shared<GenericAnimationBehavior<Callable>>(callable);
}