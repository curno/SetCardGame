#pragma once

#include "stdafx.h"
#include "../Model/Stopwatch.h"
#include "AnimationBehaviors.h"
#include "StopOperation.h"

// this is a base animation class for VisualObject
class Animation abstract
{
protected:
    ref<AnimationBehavior> Behavior_; // animation behavior
    ref<::StopOperation> StopOperation_; // stop operation
    Stopwatch Watch_; // the stopwatch to control the animation.
    bool Stoped_;
    bool DeleteWhenStopped_;
public:
    Animation();
    virtual ~Animation();
    void Start(); 
    virtual void Stop();
    
    // Duration in milliseconds.
    int readonly(Duration);
    virtual int GetDuration() { return 0; }
    
    bool readonly(IsStopped);
    bool GetIsStopped() const { return Stoped_; }

    ref<AnimationBehavior> readwrite(Behavior);
    ref<AnimationBehavior> GetBehavior() { return Behavior_; }
    void SetBehavior(const ref<AnimationBehavior> behavior) { Behavior_ = behavior; }

    ref<::StopOperation> readwrite(StopOperation);
    ref<::StopOperation> GetStopOperation() { return StopOperation_; }
    void SetStopOperation(const ref<::StopOperation> operation) { StopOperation_ = operation; }

    bool readwrite(DeleteWhenStopped);
    bool GetDeleteWhenStopped() const { return DeleteWhenStopped_; }
    void SetDeleteWhenStopped(bool delete_when_stop) { DeleteWhenStopped_ = delete_when_stop; }

    // called when WM_TIMER occurs.
    void OnTimer()
    {
        if (IsStopped)
            return;

        double process = GetCurrentProcess();

        // animation over
        if (process > 1.0 || process < 0.0)
        {
            process = min(1.0, max(0.0, process));
            OnAnimation(process);
            Stop();
            return;
        }

        OnAnimation(process);
        return;
    }

public:
    // this method perform the animation with process.
    virtual void OnAnimation(double process)
    {
        // put process in the right area.
        process = min(1.0, max(0.0, process));
        // get ratio from process using Behavior_;
        double ratio = Behavior_->GetRatio(process);

        // animate, virtual function
        OnRatio(ratio);
    }

    // this function is called to perform a snapshot of the animation
    // ratio is the progress of the animation
    virtual void OnRatio(const double ratio) { }

    // get current process according to current elapsed time.
    // this method can be rewrite to perform different effect like looping, etc.
    // if this method return a process not in [0.0, 1.0], the animation will stop.
    virtual double GetCurrentProcess()
    {
        //elapsed time
        Watch_.Stop();
        auto passed = Watch_.ElapsedMilliseconds;
        Watch_.Start();

        // get duration
        int duration = Duration;

        return static_cast<double>(passed) / duration;
    }

public:
    // utils
    static bool ValidProgress(double progress) { return progress >= 0.0 && progress <= 1.0; }
    static void AdjustProgress(double &progress) { progress = max(0.0, min(1.0, progress)); }
    
};