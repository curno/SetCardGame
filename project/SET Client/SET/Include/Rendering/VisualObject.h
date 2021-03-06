#pragma once

#include "../stdafx.h"
#include "Transformation.h"
#include "../Utils/Geometry.h"
#include "Material.h"

class VisualWidget;
// this is the abstract base class of all visual object in the program, like card, button...
class VisualObject abstract : public ::std::enable_shared_from_this<VisualObject>
{
public:
    typedef GLuint GLNameType;
protected:
    Transformation Transformation_; // the transformation of the object.
    Point Position_; // The location of the object in its parent object. 
    Dimension Size_; // The size of the object in its parent object.
    Material Material_; // The opengl material of the object
    
protected:
    virtual void RenderContent() = 0; // visual object can render its content.
    virtual void PrepareRendering(); // prepare to render.
    virtual void OnResize(const CSize &size) { }

public:
    // render for picking, generally do nothing, 
    // if a object want to be picked, or it has a child which want to be picked, override this method.
    virtual void RenderForPicking() { } 
    void Render(); // This is the Template method of rendering.
    bool readwrite(Enabled);
    bool GetEnabled() const { return Enabled_; }
    virtual void SetEnabled(bool enable) { Enabled_ = enable; }
    bool readwrite(Visible);
    bool GetVisible() const { return Visible_; }
    void SetVisible(bool visible) { Visible_ = visible; }

    Point readwrite(Position);
    Point GetPosition() const { return Position_; }
    void SetPosition(const Point &p) { Position_ = p; }

    Dimension readwrite(Size);
    Dimension GetSize() const { return Size_; }

    Transformation &GetTransformation() { return Transformation_; }
    void SetSize(const Dimension &p) 
    { 
        if (p != Size_)
        {
            Size_ = p;
            OnResize(CSize(p.Width, p.Height));
        }
    }

    // transformations.
    void Translate(double x, double y)
    {
        Transformation_.Translate(x, y, 0.0);
    }
    void Scale(double f)
    {
        Transformation_.ScaleByCenter(0, 0, 0,
            f, f, f);
    }
    void Rotate(double axis_x, double axis_y, double axis_z, double theta)
    {
        Transformation_.RotateByCenter(0, 0, 0,
            axis_x, axis_y, axis_z, theta);
    }

    VisualObject(const char *meterial = nullptr) : Enabled_(true), Visible_(true) 
    {
        if (meterial != nullptr)
            Material_ = Material::GetMaterial(meterial);
    }
    virtual ~VisualObject() { } // virtual destructor

    // for picking using Opengl or CPU
    virtual VisualWidget *GetWidgetByGLName(GLNameType name) { return nullptr; }
    virtual VisualWidget *GetWidgetByViewportPosition(const CPoint &point) { return nullptr; }
    
private:
    void SetMaterial();
    bool Enabled_;
    bool Visible_;
};
