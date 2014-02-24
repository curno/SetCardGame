#pragma once

#include "GenericAnimation.h"
#include "../Rendering/VisualObject.h"
class RotateVisualObject
{
    ref<VisualObject> Target_;
    const GLdouble AxisX_;
    const GLdouble AxisY_;
    const GLdouble AxisZ_;
    const GLdouble Theta_;
    GLdouble CurrentTheta_;
public:
    RotateVisualObject(ref<VisualObject> target, 
        GLdouble axis_x, GLdouble axis_y, GLdouble axis_z, GLdouble theta) : 
        Target_(target), 
        AxisX_(axis_x), AxisY_(axis_y), AxisZ_(axis_z), Theta_(theta), CurrentTheta_(0) { }
    double operator() (double ratio)
    {
        GLdouble theta = ratio * Theta_;
        Target_->Rotate(AxisX_, AxisY_, AxisZ_, theta - CurrentTheta_);
        CurrentTheta_ = theta;
        return ratio;
    }
};