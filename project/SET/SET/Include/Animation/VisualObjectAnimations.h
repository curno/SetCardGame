#pragma once

#include "GenericAnimation.h"
#include "../Rendering/VisualObject.h"
class Rotate
{
    Transformation &Target_; // the target transformation the animation add to
    const GLdouble AxisX_; // the rotate axis
    const GLdouble AxisY_;
    const GLdouble AxisZ_;
    const GLdouble Theta_; // the rotate angle
    const GLdouble CenterX_;
    const GLdouble CenterY_;
    const GLdouble CenterZ_;
    GLdouble CurrentTheta_;
public:
    Rotate(Transformation &target,
        GLdouble axis_x, GLdouble axis_y, GLdouble axis_z, GLdouble theta) : 
        Target_(target), 
        AxisX_(axis_x), AxisY_(axis_y), AxisZ_(axis_z), Theta_(theta), CurrentTheta_(0), CenterX_(0.0), CenterY_(0.0), CenterZ_(0.0){ }
    void operator() (double ratio)
    {
        GLdouble theta = ratio * Theta_;
        Target_.RotateByCenter(CenterX_, CenterY_, CenterZ_,
            AxisX_, AxisY_, AxisZ_, 
            theta - CurrentTheta_);
        CurrentTheta_ = theta;
    }
};

class Transform
{
    Transformation &Target_; // the target transformation the animation add to.
    Transformation DeltaTransformation_; 
    Transformation StartTransformation_; // the start value, can be different from  target
public:
    Transform(Transformation &target, Transformation target_transformation)
        : Target_(target), DeltaTransformation_(target_transformation - target), StartTransformation_(target) { }
    Transform(Transformation &target, Transformation start_transformation, Transformation target_transformation)
        : Target_(target), DeltaTransformation_(target_transformation - start_transformation), StartTransformation_(start_transformation) { }
    void operator() (double ratio)
    {
        Transformation t = StartTransformation_ + DeltaTransformation_ * ratio;
        Target_ = t;
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

class Shake
{
    Transformation &Target_;
    GLdouble AxisX_;
    GLdouble AxisY_;
    GLdouble AxisZ_;
    int Period_;
    GLdouble Swing_;
public:
    Shake(Transformation &target, GLdouble axis_x, GLdouble axis_y, GLdouble axis_z, int period = 3, GLdouble swing = PI / 8)
        : Target_(target), AxisX_(axis_x), AxisY_(axis_y), AxisZ_(axis_z), Period_(period), Swing_(swing) {}


    void operator() (double ratio)
    {
        static const double decrease = 0.5;
        int period = static_cast<int>(ratio * Period_);
        double swing = Swing_ * pow(decrease, period);
        double delta = (ratio - static_cast<double>(period) / Period_) * PI * 2 * Period_;
        double rotation = sin(delta) * swing;
        Target_ = Transformation();
        Target_.Rotate(AxisX_, AxisY_, AxisZ_, rotation);
    }
};