#include "Include/stdafx.h"

#include "Include/Animation/Animation.h"
#include "Include/Animation/AnimationManager.h"

Animation::Animation() : Stoped_(false), Behavior_(AnimationBehavior::LinearBehavior()), DeleteWhenStopped_(false)
{
}

Animation::~Animation()
{
    AnimationManager::Instance().RemoveAnimation(this); // remove from manager
}

void Animation::Stop()
{
    Stoped_ = true; Watch_.Stop();
    if (DeleteWhenStopped_)
        delete this;
}

void Animation::Start()
{
    Watch_.Start();
    AnimationManager::Instance().AddAnimation(this); // remove from manager
}
