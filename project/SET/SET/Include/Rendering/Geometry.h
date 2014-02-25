#pragma once

// this file contains several geometry structure for VisualObject

typedef int Coordinate;
struct Point;
struct Dimension
{
    Coordinate Width;
    Coordinate Height;
    Coordinate Depth;
    Dimension(Coordinate w = 0, Coordinate h = 0, Coordinate d = 0) : Width(w), Height(h), Depth(d) { }
    Dimension operator-() const
    {
        Dimension s = { -Width, -Height, -Depth };
        return s;
    }

    Dimension &operator+= (const Dimension &that)
    {
        Width += that.Width;
        Height += that.Height;
        Depth += that.Depth;
        return *this;
    }
    Dimension operator+ (const Dimension &that) const
    {
        Dimension t = *this;
        t += that;
        return t;
    }
    Dimension &operator-= (const Dimension &that)
    {
        return operator+= (-that);
    }
    Dimension operator- (const Dimension &that) const
    {
        Dimension t = *this;
        t -= that;
        return t;
    }

    bool operator== (const Dimension &that) const
    {
        return Width == that.Width && Height == that.Height && Depth == that.Depth;
    }

    bool operator!= (const Dimension &that) const
    {
        return !operator== (that);
    }
};
struct Point
{
    Coordinate X;
    Coordinate Y;
    Coordinate Z;

    Point(Coordinate x = 0, Coordinate y = 0, Coordinate z = 0) : X(x), Y(y), Z(z) { }
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

    Point &operator+= (const Dimension &that)
    {
        X += that.Width;
        Y += that.Height;
        Z += that.Depth;
    }
    Point operator+ (const Dimension &that) const
    {
        Point t = *this;
        t += that;
        return t;
    }

    bool operator== (const Point &that) const
    {
        return X == that.X && Y == that.Y && Z == that.Z;
    }

    bool operator!= (const Point &that) const
    {
        return !operator== (that);
    }
};



