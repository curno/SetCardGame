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

class Blink
{
    Material &Target_;
    Material Start_;
    Material Delta_;
    int Period_;
public:
    Blink(Material &target, const Material &start, const Material &end, int period = 3) : Target_(target), Start_(start), Period_(period)
    {
        Delta_.Ambient[0] = end.Ambient[0] - start.Ambient[0];
        Delta_.Ambient[1] = end.Ambient[1] - start.Ambient[1];
        Delta_.Ambient[2] = end.Ambient[2] - start.Ambient[2];
        Delta_.Ambient[3] = end.Ambient[3] - start.Ambient[3];

        Delta_.Diffuse[0] = end.Diffuse[0] - start.Diffuse[0];
        Delta_.Diffuse[1] = end.Diffuse[1] - start.Diffuse[1];
        Delta_.Diffuse[2] = end.Diffuse[2] - start.Diffuse[2];
        Delta_.Diffuse[3] = end.Diffuse[3] - start.Diffuse[3];

        Delta_.Specular[0] = end.Specular[0] - start.Specular[0];
        Delta_.Specular[1] = end.Specular[1] - start.Specular[1];
        Delta_.Specular[2] = end.Specular[2] - start.Specular[2];
        Delta_.Specular[3] = end.Specular[3] - start.Specular[3];

        Delta_.Emission[0] = end.Emission[0] - start.Emission[0];
        Delta_.Emission[1] = end.Emission[1] - start.Emission[1];
        Delta_.Emission[2] = end.Emission[2] - start.Emission[2];
        Delta_.Emission[3] = end.Emission[3] - start.Emission[3];

        Delta_.Shininess[0] = end.Shininess[0] - start.Shininess[0];

    }

    void operator() (double ratio)
    {
        int period = static_cast<int>(ratio * Period_);
        double delta = (ratio - static_cast<double>(period) / Period_) * PI * 2 * Period_;
        GLfloat ratio_f = static_cast<GLfloat>(abs(sin(delta)));
        Target_.Ambient[0] = Start_.Ambient[0] + Delta_.Ambient[0] * ratio_f;
        Target_.Ambient[1] = Start_.Ambient[1] + Delta_.Ambient[1] * ratio_f;
        Target_.Ambient[2] = Start_.Ambient[2] + Delta_.Ambient[2] * ratio_f;
        Target_.Ambient[3] = Start_.Ambient[3] + Delta_.Ambient[3] * ratio_f;
        Target_.Diffuse[0] = Start_.Diffuse[0] + Delta_.Diffuse[0] * ratio_f;
        Target_.Diffuse[1] = Start_.Diffuse[1] + Delta_.Diffuse[1] * ratio_f;
        Target_.Diffuse[2] = Start_.Diffuse[2] + Delta_.Diffuse[2] * ratio_f;
        Target_.Diffuse[3] = Start_.Diffuse[3] + Delta_.Diffuse[3] * ratio_f;
        Target_.Specular[0] = Start_.Specular[0] + Delta_.Specular[0] * ratio_f;
        Target_.Specular[1] = Start_.Specular[1] + Delta_.Specular[1] * ratio_f;
        Target_.Specular[2] = Start_.Specular[2] + Delta_.Specular[2] * ratio_f;
        Target_.Specular[3] = Start_.Specular[3] + Delta_.Specular[3] * ratio_f;
        Target_.Emission[0] = Start_.Emission[0] + Delta_.Emission[0] * ratio_f;
        Target_.Emission[1] = Start_.Emission[1] + Delta_.Emission[1] * ratio_f;
        Target_.Emission[2] = Start_.Emission[2] + Delta_.Emission[2] * ratio_f;
        Target_.Emission[3] = Start_.Emission[3] + Delta_.Emission[3] * ratio_f;
        Target_.Shininess[0] = Start_.Shininess[0] + Delta_.Shininess[0] * ratio_f;
    }
};