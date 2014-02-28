#pragma once

#include "stdafx.h"
#include "Geometry.h"
// static class
class Random
{
public:
    static double NextDouble()
    {
        static std::default_random_engine generator;
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        return distribution(generator);
    }


    

};