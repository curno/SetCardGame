#include "Include/stdafx.h"

#include "Include/Animation/Animation.h"
#include "Include/Animation/AnimationManager.h"

Animation::Animation() : Stoped_(false), Behavior_(AnimationBehavior::LinearBehavior())
{
    AnimationManager::Instance().AddAnimation(this); // add to manager
}

Animation::~Animation()
{
    AnimationManager::Instance().RemoveAnimation(this); // remove from manager
}
