#pragma once

#include "../stdafx.h"

// This is a stopwatch class to get the elapsed time during the game.
class Stopwatch
{
    CTime StartTime_;
    CTimeSpan ElapsedTime_;
    enum class State
    {
        Stopped, Activated
    } State_;
public:
    Stopwatch()
    {
        ElapsedTime_ = 0;
        State_ = State::Stopped;
    }
    void Start()
    {
        if (State_ == State::Stopped)
        {
            StartTime_ = CTime::GetCurrentTime();
            State_ = State::Activated;
        }
    }
    void Stop()
    {
        if (State_ == State::Activated)
        {
            ElapsedTime_ += CTime::GetCurrentTime() - StartTime_;
            State_ = State::Stopped;
        }
    }
    void Clear()
    {
        Stop();
        ElapsedTime_ = 0;
    }

    void Restart()
    {
        Clear();
        Start();
    }

    CTimeSpan readonly(TimeElapsed);
    CTimeSpan GetTimeElapsed() 
    {
        if (State_ == State::Activated)
        {
            Stop();
            CTimeSpan retval = ElapsedTime_;
            Start();
            return retval;
        }
        return ElapsedTime_;
    }
};