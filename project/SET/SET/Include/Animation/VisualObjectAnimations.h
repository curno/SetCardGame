#pragma once

#include "GenericAnimation.h"
#include "../Rendering/VisualObject.h"
class RotateVisualObject
{
    VisualObject *Target_;
    const GLdouble AxisX_;
    const GLdouble AxisY_;
    const GLdouble AxisZ_;
    const GLdouble Theta_;
    GLdouble CurrentTheta_;
public:
    RotateVisualObject(VisualObject *target, 
        GLdouble axis_x, GLdouble axis_y, GLdouble axis_z, GLdouble theta) : 
        Target_(target), 
        AxisX_(axis_x), AxisY_(axis_y), AxisZ_(axis_z), Theta_(theta), CurrentTheta_(0) { }
    void operator() (double ratio)
    {
        GLdouble theta = ratio * Theta_;
        Target_->Rotate(AxisX_, AxisY_, AxisZ_, theta - CurrentTheta_);
        CurrentTheta_ = theta;
    }
};

class TransformVisualObject
{
    VisualObject *Target_;
    Transformation DeltaTransformation_;
    Transformation StartTransformation_;
public:
    TransformVisualObject(VisualObject *target, Transformation target_transformation)
        : Target_(target), DeltaTransformation_(target_transformation - target->GetTransformation()), StartTransformation_(target->GetTransformation()) { }
    TransformVisualObject(VisualObject *target, Transformation start_transformation, Transformation target_transformation)
        : Target_(target), DeltaTransformation_(target_transformation - start_transformation), StartTransformation_(start_transformation) { }
    void operator() (double ratio)
    {
        Transformation t = StartTransformation_ + DeltaTransformation_ * ratio;
        Target_->GetTransformation() = t;
    }
};

class MoveVisualObject
{
    VisualObject *Target_;
    Point StartPosition_;
    Point DeltaPosition_;
public:
    MoveVisualObject(VisualObject *target, Point target_position)
        : Target_(target), DeltaPosition_(target_position - target->Position), StartPosition_(target->Position) { }

    MoveVisualObject(VisualObject *target, Point start_position, Point target_position)
        : Target_(target), DeltaPosition_(target_position - start_position), StartPosition_(start_position) { }

    void operator() (double ratio)
    {
        Point t = StartPosition_ + Point(static_cast<Coordinate>(DeltaPosition_.X * ratio), 
            static_cast<Coordinate>(DeltaPosition_.Y * ratio), 
            static_cast<Coordinate>(DeltaPosition_.Z * ratio));
        Target_->Position = t;
    }

};