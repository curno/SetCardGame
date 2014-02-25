#pragma once

#include "Animation.h"

// this class manage all animations in the program.
// it is a singleton
class AnimationManager
{
    ::std::unordered_set<Animation *> Animations_; // all animation pointers.
private:
    AnimationManager() { } // private constructor.
    AnimationManager(const AnimationManager &); // suppress copy constructor
    AnimationManager &operator= (const AnimationManager &); // suppress assign operator
public:
    void AddAnimation(Animation *animation)
    {
        if (animation == nullptr)
            return;
        // already in the collection, error
        assert(Animations_.find(animation) == Animations_.end());

        Animations_.insert(animation);
    }

    void RemoveAnimation(Animation *animation)
    {
        if (animation == nullptr)
            return;

        auto i = Animations_.find(animation);
        // not in the collection error
        assert(i != Animations_.end());

        Animations_.erase(i);
    }

    void PerformAllAnimation()
    {
        for each (Animation *animation in Animations_)
        {
            if (!animation->IsStopped)
                animation->OnTimer();
        }
    }
    ~AnimationManager()
    {
    }

    // singleton
    static AnimationManager &Instance()
    {
        static AnimationManager Instance_;
        return Instance_;
    }
};