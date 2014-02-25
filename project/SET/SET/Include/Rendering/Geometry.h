#pragma once

// this file contains several geometry structure for VisualObject

typedef int Coordinate;
struct Point;
struct Size
{
    Coordinate Width;
    Coordinate Height;
    Coordinate Depth;
    Size operator-() const
    {
        Size s = { -Width, -Height, -Depth };
        return s;
    }

    Size &operator+= (const Size &that)
    {
        Width += that.Width;
        Height += that.Height;
        Depth += that.Depth;
        return *this;
    }
    Size operator+ (const Size &that) const
    {
        Size t = *this;
        t += that;
        return t;
    }
    Size &operator-= (const Size &that)
    {
        return operator+= (-that);
    }
    Size operator- (const Size &that) const
    {
        Size t = *this;
        t -= that;
        return t;
    }

};
struct Point
{
    Coordinate X;
    Coordinate Y;
    Coordinate Z;

    Point operator- () const
    {
        Point p = { -X, -Y, -Z };
        return p;
    }
    Point &operator+= (const Point &that)
    {
        X += that.X;
        Y += that.Y;
        Z += that.Z;
        return *this;
    }
    Point operator+ (const Point &that) const 
    {
        Point t = *this;
        t += that;
        return t;
    }
    Point &operator-= (const Point &that) 
    { 
        return operator+= (-that); 
    }
    Point operator- (const Point &that) const
    {
        Point t = *this;
        t -= that;
        return t;
    }

    Point &operator+= (const Size &that)
    {
        X += that.Width;
        Y += that.Height;
        Z += that.Depth;
    }
    Point operator+ (const Size &that) const
    {
        Point t = *this;
        t += that;
        return t;
    }
};



