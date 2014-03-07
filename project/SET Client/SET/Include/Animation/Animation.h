#pragma once

#include "stdafx.h"
#include "../Utils/Stopwatch.h"
#include "AnimationBehaviors.h"
#include "../Rendering/Operation.h"

// this is a base animation class for VisualObject
class Animation abstract
{
protected:
    ref<AnimationBehavior> Behavior_; // animation behavior
    ref<::Operation> StopOperation_; // stop operation
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
    virtual int GetDuration();
    
    bool readonly(IsStopped);
    bool GetIsStopped() const;

    ref<AnimationBehavior> readwrite(Behavior);
    ref<AnimationBehavior> GetBehavior();
    void SetBehavior(const ref<AnimationBehavior> behavior);

    ref<::Operation> readwrite(StopOperation);
    ref<::Operation> GetStopOperation();
    void SetStopOperation(const ref<::Operation> operation);

    bool readwrite(DeleteWhenStopped);
    bool GetDeleteWhenStopped() const;
    void SetDeleteWhenStopped(bool delete_when_stop);

    // called when WM_TIMER occurs.
    void OnTimer();

public:
    // this method perform the animation with process.
    virtual void OnAnimation(double process);

    // this function is called to perform a snapshot of the animation
    // ratio is the progress of the animation
    virtual void OnRatio(const double ratio);

    // get current process according to current elapsed time.
    // this method can be rewrite to perform different effect like looping, etc.
    // if this method return a process not in [0.0, 1.0], the animation will stop.
    virtual double GetCurrentProcess();

public:
    // utils
    // check if the progress is in [0.0, 1.0]
    static bool ValidProgress(double progress);
    // adjuct the progress to be in [0.0, 1.0]
    static void AdjustProgress(double &progress);
    
};