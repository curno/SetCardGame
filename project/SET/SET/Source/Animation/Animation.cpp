#include "Include/stdafx.h"

#include "Include/Animation/Animation.h"
#include "Include/Animation/AnimationManager.h"

Animation::Animation() : Stoped_(false), Behavior_(AnimationBehavior::LinearBehavior()), DeleteWhenStopped_(false)
{
}

Animation::~Animation()
{
    Stop();
    AnimationManager::Instance().RemoveAnimation(this); // remove from manager
}

void Animation::Stop()
{
    if (!Stoped_)
    {
        Stoped_ = true; Watch_.Stop();
        if (StopOperation_ != nullptr)
            (*StopOperation_)(); // call stop operation.
        if (DeleteWhenStopped_)
            delete this;
    }
}

void Animation::Start()
{
    Watch_.Start();
    AnimationManager::Instance().AddAnimation(this); // remove from manager
}
