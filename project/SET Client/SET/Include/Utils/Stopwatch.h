#pragma once

#include "../stdafx.h"

// This is a stopwatch class to get the elapsed time during the game.
class Stopwatch
{
public:

    Stopwatch() : IsRunning_(false), Elapsed_(0) { }

    static const long long Frequency;
    static const bool IsHighResolution;
private:

    static bool GetIsHighResolution()
    {
        LARGE_INTEGER tmp;
        return ::QueryPerformanceFrequency(&tmp) == TRUE;
    }
    static long long GetFrequecy()
    {
        LARGE_INTEGER tmp;
        if (::QueryPerformanceFrequency(&tmp))
            return (long long)tmp.QuadPart / 1000;
        else
            return (long long)1;
    }

public:
    long long GetElapsedMilliseconds() 
    {
        if (IsRunning_)
        {
            Stop();
            auto retval = (long long)(Elapsed_ / ((double)Stopwatch::Frequency));
            Start();
            return retval;
        }
        return (long long)(Elapsed_ / ((double)Stopwatch::Frequency));
    }
    void SetElapsedMilliseconds(long long milliseconds)
    {
        Elapsed_ = milliseconds * Stopwatch::Frequency;
        StartTimeStamp_ = Stopwatch::GetTimestamp();
    }
    long long readwrite(ElapsedMilliseconds);

    bool GetIsRunning() const
    {
        return IsRunning_;
    }
    bool readonly(IsRunning);

    void Reset()
    {
        Stop();
        Elapsed_ = 0;
    }

    void Start()
    {
        if (!IsRunning_)
        {
            IsRunning_ = true;
            StartTimeStamp_ = Stopwatch::GetTimestamp();
        }
    }

    void Restart()
    {
        Reset();
        Start();
    }

    void Stop() 
    {
        if (IsRunning_)
        {
            IsRunning_ = false;
            long long tmp_timestamp = StartTimeStamp_;
            StartTimeStamp_ = Stopwatch::GetTimestamp();
            Elapsed_ += StartTimeStamp_ - tmp_timestamp;
        }
    }
    static long long GetTimestamp()
    {
        if (Stopwatch::IsHighResolution)
        {
            LARGE_INTEGER tmp;
            ::QueryPerformanceCounter(&tmp);
            return (long long)(tmp.QuadPart);
        }
        else
        {
            return GetTickCount();
        }
    }

private:
    bool IsRunning_;
    long long StartTimeStamp_;
    long long Elapsed_;
};
