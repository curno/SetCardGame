#include "Include/stdafx.h"

#include "Include/Animation/SequentialAnimation.h"

void SequentialAnimation::OnAnimation(double progress)
{
    __super::OnRatio(progress);
    double current_step = Duration * progress;

    // perform current animation.
    int current = 0;
    for (auto i = Animations_.begin(); i != Animations_.end(); ++i)
    {

        int old_current = current;
        if (current <= current_step)
        {
            if (!(*i)->IsStopped)
            {
                double sub_progress = (current_step - old_current) / (*i)->Duration;
                (*i)->OnAnimation(sub_progress);
                if (!Animation::ValidProgress(sub_progress))
                    (*i)->Stop();
            }
        }
        else
            break;
        current += (*i)->Duration;

    }
}

int SequentialAnimation::GetDuration()
{
    int retval = 0;
    for each (ref<Animation> animation in Animations_)
        retval += animation->Duration;
    return retval;
}

void SequentialAnimation::AddAnimation(ref<Animation> animation)
{
    // already in the group.
    if (::std::find(Animations_.begin(), Animations_.end(), animation) != Animations_.end())
        return;
    Animations_.push_back(animation);

    // remove from the AnimationManager, because animation is not independent anymore
    AnimationManager::Instance().RemoveAnimation(animation.get());
}

void SequentialAnimation::Stop()
{
    for each (ref<Animation> animation in Animations_)
        animation->Stop();
    __super::Stop();
}

SequentialAnimation::~SequentialAnimation()
{
    Stop();
}
