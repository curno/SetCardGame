#include "Include/stdafx.h"

#include "Include/Animation/Animation.h"
#include "Include/Animation/AnimationManager.h"

Animation::Animation() : Stoped_(false), Behavior_(AnimationBehavior::LinearBehavior()), DeleteWhenStopped_(false)
{
}

Animation::~Animation()
{
    Stop();
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
        AnimationManager::Instance().RemoveAnimation(this); // remove from manager

    }
}

void Animation::Start()
{
    Watch_.Start();
    AnimationManager::Instance().AddAnimation(this); // remove from manager
}

void Animation::AdjustProgress(double &progress)
{
    progress = ::std::max(0.0, ::std::min(1.0, progress));
}

bool Animation::ValidProgress(double progress)
{
    return progress >= 0.0 && progress <= 1.0;
}

double Animation::GetCurrentProcess()
{
    //elapsed time
    Watch_.Stop();
    auto passed = Watch_.ElapsedMilliseconds;
    Watch_.Start();

    // get duration
    int duration = Duration;

    return static_cast<double>(passed) / duration;
}

void Animation::OnRatio(const double ratio)
{

}

void Animation::OnAnimation(double process)
{
    // put process in the right area.
    process = ::std::min(1.0, ::std::max(0.0, process));
    // get ratio from process using Behavior_;
    double ratio = Behavior_->GetRatio(process);

    // animate, virtual function
    OnRatio(ratio);
}

void Animation::OnTimer()
{
    if (IsStopped)
        return;

    double process = GetCurrentProcess();

    // animation over
    if (process > 1.0 || process < 0.0)
    {
        process = ::std::min(1.0, ::std::max(0.0, process));
        OnAnimation(process);
        Stop();
        return;
    }

    OnAnimation(process);
    return;
}

void Animation::SetDeleteWhenStopped(bool delete_when_stop)
{
    DeleteWhenStopped_ = delete_when_stop;
}

bool Animation::GetDeleteWhenStopped() const
{
    return DeleteWhenStopped_;
}

void Animation::SetStopOperation(const ref<::Operation> operation)
{
    StopOperation_ = operation;
}

ref<::Operation> Animation::GetStopOperation()
{
    return StopOperation_;
}

void Animation::SetBehavior(const ref<AnimationBehavior> behavior)
{
    Behavior_ = behavior;
}

ref<AnimationBehavior> Animation::GetBehavior()
{
    return Behavior_;
}

bool Animation::GetIsStopped() const
{
    return Stoped_;
}

int Animation::GetDuration()
{
    return 0;
}
