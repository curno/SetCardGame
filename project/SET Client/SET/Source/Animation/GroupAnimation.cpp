#include "Include/stdafx.h"

#include "Include/Animation/GroupAnimation.h"

void GroupAnimation::OnAnimation(double progress)
{
    __super::OnAnimation(progress);
    double current_step = Duration * progress;

    // perform each animation in parallel with proper ratio.
    for each (ref<Animation> animation in Animations_)
    {
        if (!animation->IsStopped)
        {
            double sub_progress = current_step / animation->Duration;
            animation->OnAnimation(sub_progress);
            if (!Animation::ValidProgress(sub_progress))
                animation->Stop();
        }
    }
}

int GroupAnimation::GetDuration()
{
    int retval = 0;
    for each (ref<Animation> animation in Animations_)
        retval = ::std::max(retval, animation->Duration);
    return retval;
}

void GroupAnimation::AddAnimation(ref<Animation> animation)
{
    // already in the group.
    if (::std::find(Animations_.begin(), Animations_.end(), animation) != Animations_.end())
        return;
    Animations_.push_back(animation);

    // remove from the AnimationManager, because animation is not independent anymore
    AnimationManager::Instance().RemoveAnimation(animation.get());
}

void GroupAnimation::Stop()
{
    for each (ref<Animation> animation in Animations_)
        animation->Stop();
    __super::Stop();
}

GroupAnimation::~GroupAnimation()
{
    Stop();
}
