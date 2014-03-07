#include "Include/stdafx.h"

#include "Include/Animation/AnimationManager.h"

AnimationManager & AnimationManager::Instance()
{
    static AnimationManager Instance_;
    return Instance_;
}

AnimationManager::~AnimationManager()
{

}

void AnimationManager::StopAllAnimation()
{
    auto copy = Animations_;
    for (auto i = copy.begin(); i != copy.end(); ++i)
        (*i)->Stop();
    Animations_.clear();
}

void AnimationManager::PerformAllAnimation()
{
    if (Animations_.empty())
        return;
    auto copy = Animations_;
    for (auto i = copy.begin(); i != copy.end(); ++i)
    {
        if (!(*i)->IsStopped)
            (*i)->OnTimer();
    }
}

void AnimationManager::RemoveAnimation(Animation *animation)
{
    if (animation == nullptr)
        return;

    auto i = ::std::find(Animations_.begin(), Animations_.end(), animation);
    // not in the collection error
    if (i == Animations_.end())
        return;

    Animations_.erase(i);
}

void AnimationManager::AddAnimation(Animation *animation)
{
    if (animation == nullptr)
        return;

    // already in the collection, error
    if (::std::find(Animations_.begin(), Animations_.end(), animation) != Animations_.end())
        return;

    Animations_.push_back(animation);
}

AnimationManager::AnimationManager()
{

}
