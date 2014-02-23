#pragma once

#include "stdafx.h"
// this class represents a transformation in opengl rendering process.
// It uses a GLdouble array of 16 size to represent a transformation.
// It contains some useful transformations.

class Transformation
{
private:
    typedef GLdouble *TransformationDataType;
    GLdouble Data_[16];

public:
    void SetData(
        GLdouble d00, GLdouble d01, GLdouble d02, GLdouble d03,
        GLdouble d10, GLdouble d11, GLdouble d12, GLdouble d13,
        GLdouble d20, GLdouble d21, GLdouble d22, GLdouble d23,
        GLdouble d30, GLdouble d31, GLdouble d32, GLdouble d33)
    {
        Data_[0] = d00; Data_[4] = d01; Data_[8] = d02; Data_[12] = d03;
        Data_[1] = d10; Data_[5] = d11; Data_[9] = d12; Data_[13] = d13;
        Data_[2] = d20; Data_[6] = d21; Data_[10] = d22; Data_[14] = d23;
        Data_[3] = d30; Data_[7] = d31; Data_[11] = d32; Data_[15] = d33;
    }
    TransformationDataType readonly(Data);
    TransformationDataType GetData() { return Data_; }
    Transformation(
        GLdouble d00, GLdouble d01, GLdouble d02, GLdouble d03,
        GLdouble d10, GLdouble d11, GLdouble d12, GLdouble d13,
        GLdouble d20, GLdouble d21, GLdouble d22, GLdouble d23,
        GLdouble d30, GLdouble d31, GLdouble d32, GLdouble d33)
    {
        SetData(
            d00, d01, d02, d03,
            d10, d11, d12, d13,
            d20, d21, d22, d23,
            d30, d31, d32, d33);
    }

    Transformation()
    {
        SetData(
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);
    }

    Transformation operator* (const Transformation &right)
    {
        #define MULT(ROW, COLUMN) \
            Data_[ROW] * right.Data_[COLUMN * 4] + Data_[ROW + 4] * right.Data_[COLUMN * 4 + 1] + Data_[ROW + 8] * right.Data_[COLUMN * 4 + 2] + Data_[ROW + 12] * right.Data_[COLUMN * 4 + 3]
        return Transformation(
            MULT(0, 0), MULT(0, 1), MULT(0, 2), MULT(0, 3),
            MULT(1, 0), MULT(1, 1), MULT(1, 2), MULT(1, 3),
            MULT(2, 0), MULT(2, 1), MULT(2, 2), MULT(2, 3),
            MULT(3, 0), MULT(3, 1), MULT(3, 2), MULT(3, 3)
            );
    }

    Transformation &operator*= (const Transformation &right)
    {
        *this = operator *(right);
        return *this;
    }

    void Translate(GLdouble x, GLdouble y, GLdouble z)
    {
        *this = Transformation(
            1.0, 0.0, 0.0, x,
            0.0, 1.0, 0.0, y,
            0.0, 0.0, 1.0, z,
            0.0, 0.0, 0.0, 1.0
            ) * (*this);
    }

    void Scale(GLdouble x, GLdouble y, GLdouble z)
    {
        *this = Transformation(
            x, 0.0, 0.0, 0.0,
            0.0, y, 0.0, 0.0,
            0.0, 0.0, z, 0.0,
            0.0, 0.0, 0.0, 1.0
            ) * (*this);
    }
};