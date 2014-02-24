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
    void OnTimer()
    {
        assert(!Stoped_);
        Watch_.Stop();
        int passed = Watch_.ElapsedMilliseconds;
        Watch_.Start();
        int duration = Duration;
        double ratio = static_cast<double>(passed) / duration;
        if (ratio >= 1.0)
        {
            ratio = 1.0;
            Stop();
        }
        OnAnimationStep(ratio);
    }
protected:
    virtual void OnAnimationStep(double ratio) { }
};