#pragma once

#include "VisualObject.h"

// this is a mouse-clickable object with hover effect.
class VisualWidget : public VisualObject
{
    GLNameType Name_; // The opengl name of the widget object.
    CRect ViewportRect_;
public:
    VisualWidget(const char *meterial);

    void SetMaterialEmission(GLfloat emission_value);
    void SetEnabled(bool enable) override;

    virtual VisualWidget *GetWidgetByGLName(GLNameType name) override;
    virtual VisualWidget *GetWidgetByViewportPosition(const CPoint &point) override;

    // events
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
    virtual void OnMouseMove() { }
    virtual void OnMouseButtonDown() { } // visual widget handle mouse button down.

    virtual void RenderForPicking() override; 
protected:
    void UpdateMaterialEmission();
    virtual void RenderPickingContent();
    virtual void RenderContent() override;

protected:
    GLfloat EmissionHighlight_;
    GLfloat EmissionNormal_;
    GLfloat EmissionDisabled_;

private:
    void UpdateViewportRect();

    static GLNameType CreateName();

};