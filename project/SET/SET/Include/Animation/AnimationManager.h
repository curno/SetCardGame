#pragma once

#include "Animation.h"

// this class manage all animations in the program.
// it is a singleton
class AnimationManager
{
    ::std::vector<Animation *> Animations_; // all animation pointers.
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
        if (::std::find(Animations_.begin(), Animations_.end(), animation) != Animations_.end())
            return;

        Animations_.push_back(animation);
    }

    void RemoveAnimation(Animation *animation)
    {
        if (animation == nullptr)
            return;

        auto i = ::std::find(Animations_.begin(), Animations_.end(), animation);
        // not in the collection error
        if (i == Animations_.end())
            return;

        Animations_.erase(i);
    }

    void PerformAllAnimation()
    {
        if (Animations_.empty())
            return;
        for (int i = static_cast<int>(Animations_.size())- 1; i >= 0; --i)
        {
            auto animation = Animations_[i];
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