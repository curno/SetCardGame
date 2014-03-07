#pragma once

#include "Animation.h"

// this class manage all animations in the program.
// it is a singleton
class AnimationManager
{
    ::std::vector<Animation *> Animations_; // all animation pointers.
private:
    AnimationManager(); // private constructor.
    AnimationManager(const AnimationManager &); // suppress copy constructor
    AnimationManager &operator= (const AnimationManager &); // suppress assign operator
public:
    void AddAnimation(Animation *animation);
    void RemoveAnimation(Animation *animation);

    void PerformAllAnimation();
    void StopAllAnimation();
    ~AnimationManager();

    // singleton
    static AnimationManager &Instance();
};