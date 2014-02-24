#pragma once

#include "../stdafx.h"
#include "Transformation.h"

// this is the abstract base class of all visual object in the program, like card, deck, button...
class VisualObject abstract
{
public:
    typedef GLuint GLNameType;
protected:
    Transformation Transformation_; // the transformation of the object.
    CPoint Position_; // The location of the object in its parent object. 
    CSize Size_; // The size of the object in its parent object.
    GLNameType Name_; // The name of the opengl display object.
protected:
    virtual void RenderContent() = 0; // visual object can render its content.
    virtual void PrepareRendering() { } // prepare to render.
    virtual void OnResize(const CSize &size) { }

    static GLNameType CreateName();
public:
    void Render(); // This is the Template method of rendering.
    CPoint readwrite(Position);
    CPoint GetPosition() const { return Position_; }
    void SetPosition(const CPoint &p) { Position_ = p; }

    CSize readwrite(Size);
    CSize GetSize() const { return Size_; }
    void SetSize(const CSize &p) 
    { 
        if (p != Size_)
        {
            Size_ = p;
            OnResize(GetSize());
        }
    }

    void Translate(double x, double y)
    {
        Transformation_.Translate(x, y, 0.0);
    }

    void Scale(double f)
    {
        Transformation_.ScaleByCenter(Size.cx / 2.0, Size.cy / 2.0, 0.0,
            f, f, f);
    }

    void Rotate(double axis_x, double axis_y, double axis_z, double theta)
    {
        Transformation_.RotateByCenter(Size.cx / 2.0, Size.cy / 2.0, 0.0,
            axis_x, axis_y, axis_z, theta);
    }

    VisualObject() : Name_(CreateName()) { }
    virtual ~VisualObject() { } // virtual destructor
    virtual VisualObject *GetObjectByGLName(GLNameType name);

    virtual void OnMouseEnter() { }
    virtual void OnMouseLeave() { }
    virtual void OnMouseMove() { }
    virtual void OnMouseButtonDown() { } // visual object handle mouse button down.

};
