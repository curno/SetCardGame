#include "Include/stdafx.h"

#include "Include/Rendering/VisualWidget.h"

VisualWidget::VisualWidget(const char *meterial) : VisualObject(meterial), Name_(CreateName()), EmissionHighlight_(0.7f), EmissionNormal_(0.3f), EmissionDisabled_(0.0f)
{
    UpdateMaterialEmission();
}

void VisualWidget::OnMouseEnter()
{
    if (Enabled)
        SetMaterialEmission(EmissionHighlight_);
}

void VisualWidget::OnMouseLeave()
{
    if (Enabled)
        SetMaterialEmission(EmissionNormal_);
}

void VisualWidget::SetMaterialEmission(GLfloat emission_value)
{
    GLfloat emission[] = { emission_value, emission_value, emission_value, 1.0f };
    Material_.SetData(Material::Parameter::Emission, emission);
}

void VisualWidget::UpdateMaterialEmission()
{
    if (Enabled)
        SetMaterialEmission(EmissionNormal_);
    else
        SetMaterialEmission(EmissionDisabled_);
}

void VisualWidget::SetEnabled(bool enable)
{
    __super::SetEnabled(enable);
    UpdateMaterialEmission();
}

void VisualWidget::RenderForPicking()
{
    // picking only when visible.
    if (Visible)
    {
        glLoadName(Name_);
        glMatrixMode(GL_MODELVIEW); // Select ModelView matrix.
        glPushMatrix(); // save current matrix
        glMultMatrixd(Transformation_.Data); // add current transform to current matrix
        RenderPickingContent(); // render
        glPopMatrix(); // restore the saved matrix.
    }
}

void VisualWidget::RenderPickingContent()
{
    Point p0(-Size.Width / 2, -Size.Height / 2, Size.Depth / 2);
    Point p1(Size.Width / 2, Size.Height / 2, Size.Depth / 2);
    glBegin(GL_QUADS);
    glVertex3d(p0.X, p0.Y, p0.Z);
    glVertex3d(p1.X, p0.Y, p0.Z);
    glVertex3d(p1.X, p1.Y, p0.Z);
    glVertex3d(p0.X, p1.Y, p0.Z);
    glEnd();


}

VisualWidget *VisualWidget::GetWidgetByGLName(GLNameType name)
{
    if (Name_ == name) // if self has the name, return this.
        return this;
    else
        return nullptr;
}

VisualWidget *VisualWidget::GetWidgetByViewportPosition(const CPoint &point)
{
    if (ViewportRect_.left <= point.x && ViewportRect_.right >= point.x &&
        ViewportRect_.top <= point.y && ViewportRect_.bottom >= point.y)
        return this;
    return nullptr;
}

VisualObject::GLNameType VisualWidget::CreateName()
{
    static GLNameType Name = 0x21; // beginning name
    Name++;
    return Name;
}

void VisualWidget::UpdateViewportRect()
{
    Point p0(-Size.Width / 2, -Size.Height / 2, Size.Depth / 2);
    Point p1(Size.Width / 2, Size.Height / 2, Size.Depth / 2);
    // update view port rect.
    GLdouble model_view_matrix[16];
    GLdouble projection_matrix[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, model_view_matrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    double minx = ::std::numeric_limits<double>::max(), maxx = 0, miny = ::std::numeric_limits<double>::max(), maxy = 0;
    double x, y, z;
    gluProject(p0.X, p0.Y, p0.Z, model_view_matrix, projection_matrix, viewport, &x, &y, &z);
    minx = ::std::min(minx, x); maxx = ::std::max(maxx, x); miny = ::std::min(miny, y); maxy = ::std::max(maxy, y);
    gluProject(p1.X, p0.Y, p0.Z, model_view_matrix, projection_matrix, viewport, &x, &y, &z);
    minx = ::std::min(minx, x); maxx = ::std::max(maxx, x); miny = ::std::min(miny, y); maxy = ::std::max(maxy, y);
    gluProject(p1.X, p1.Y, p0.Z, model_view_matrix, projection_matrix, viewport, &x, &y, &z);
    minx = ::std::min(minx, x); maxx = ::std::max(maxx, x); miny = ::std::min(miny, y); maxy = ::std::max(maxy, y);
    gluProject(p0.X, p1.Y, p0.Z, model_view_matrix, projection_matrix, viewport, &x, &y, &z);
    minx = ::std::min(minx, x); maxx = ::std::max(maxx, x); miny = ::std::min(miny, y); maxy = ::std::max(maxy, y);

    ViewportRect_.SetRect(static_cast<int>(minx), static_cast<int>(miny),
        static_cast<int>(maxx), static_cast<int>(maxy));
}

void VisualWidget::RenderContent()
{
    UpdateViewportRect();
}


