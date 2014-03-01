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

    Dimension &operator+= (const Dimension &right)
    {
        Width += right.Width;
        Height += right.Height;
        Depth += right.Depth;
        return *this;
    }
    Dimension operator+ (const Dimension &right) const
    {
        Dimension t = *this;
        t += right;
        return t;
    }
    Dimension &operator-= (const Dimension &right)
    {
        return operator+= (-right);
    }
    Dimension operator- (const Dimension &right) const
    {
        Dimension t = *this;
        t -= right;
        return t;
    }

    bool operator== (const Dimension &right) const
    {
        return Width == right.Width && Height == right.Height && Depth == right.Depth;
    }

    bool operator!= (const Dimension &right) const
    {
        return !operator== (right);
    }
};
struct Point
{
    Coordinate X;
    Coordinate Y;
    Coordinate Z;

    Point(Coordinate x = 0, Coordinate y = 0, Coordinate z = 0) 
        : X(x), Y(y), Z(z)
    {
    }
    Point operator- () const
    {
        Point p = { -X, -Y, -Z };
        return p;
    }
    Point &operator+= (const Point &right)
    {
        X += right.X;
        Y += right.Y;
        Z += right.Z;
        return *this;
    }
    Point operator+ (const Point &right) const 
    {
        Point t = *this;
        t += right;
        return t;
    }
    Point &operator-= (const Point &right) 
    { 
        return operator+= (-right); 
    }
    Point operator- (const Point &right) const
    {
        Point t = *this;
        t -= right;
        return t;
    }

    Point &operator+= (const Dimension &right)
    {
        X += right.Width;
        Y += right.Height;
        Z += right.Depth;
        return *this;
    }
    Point operator+ (const Dimension &right) const
    {
        Point t = *this;
        t += right;
        return t;
    }

    bool operator== (const Point &right) const
    {
        return X == right.X && Y == right.Y && Z == right.Z;
    }

    bool operator!= (const Point &right) const
    {
        return !operator== (right);
    }

    double Length() const
    {
        return sqrt(X * X + Y * Y + Z * Z);
    }
};



