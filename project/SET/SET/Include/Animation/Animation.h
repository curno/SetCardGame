#pragma once

#include "stdafx.h"
#include "../Model/Stopwatch.h"

// this is a base animation class for VisualObject
class Animation abstract
{
    Stopwatch Watch_; // the stopwatch to control the animation.
    bool Stoped_;
public:
    Animation() : Stoped_(false) { }
    virtual void Start() { Watch_.Start(); } 
    virtual void Stop()  { Stoped_ = true; Watch_.Stop();  }
    
    // Duration in milliseconds.
    int readonly(Duration);
    virtual int GetDuration() { return 0; }
    
    bool readonly(IsStopped);
    bool GetIsStopped() const { return Stoped_; }

    // called when WM_TIMER occurs.
    void OnTimer()
    {
        assert(!Stoped_);

        //elapsed time
        Watch_.Stop();
        int passed = Watch_.ElapsedMilliseconds;
        Watch_.Start();

        // get duration
        int duration = Duration;

        double ratio = static_cast<double>(passed) / duration;

        // animation over
        if (ratio >= 1.0)
            ratio = 1.0;

        // animate
        double new_ratio = OnAnimationStep(ratio);

        // if there is ratio adjustment
        if (new_ratio != ratio)
        {
            new_ratio = max(0.0, min(1.0, new_ratio));
            Watch_.SetElapsedMilliseconds(new_ratio * duration);
        }

        // if over
        if (new_ratio >= 1.0)
            Stop();
    }
public:
    // this function is called to perform a snapshot of the animation
    // ratio is the progress of the animation
    // user can adjust the ratio by return the new animation ratio.
    // for most of the time, just return old ratio.
    virtual double OnAnimationStep(const double ratio) { }
};