#pragma once

#include "../stdafx.h"

// this is the abstract base class of all visual object in the program, like card, deck, button...
class VisualObject abstract
{
    CPoint Position_; // The location of the object in its parent object. 
    CSize Size_; // The size of the object in its parent object.
protected:
    virtual void RenderContent() = 0; // visual object can render its content.
    virtual void OnMouseButtonDown() { } // visual object handle mouse button down.
    virtual void PrepareRendering() { } // prepare to render.
    virtual void OnResize(const CSize &size) { }
public:
    void Render(); // This is the Template method of rendering.
    CPoint readwrite(Position);
    CPoint GetPosition() const { return Position_; }
    void SetPosition(const CPoint &p) { Position_ = p; }

    CSize readwrite(Size);
    CSize GetSize() const { return Size_; }
    void SetSize(const CSize &p) 
    { 
        Size_ = p; 
        OnResize(GetSize()); 
    }
};