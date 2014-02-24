#include "Include/stdafx.h"

#include "Include/Rendering/VisualObject.h"


void VisualObject::Render()
{
    glLoadName(Name_);
    glMatrixMode(GL_MODELVIEW); // Select ModelView matrix.
    glPushMatrix(); // save current matrix
    glMultMatrixd(Transformation_.Data); // add current transform to current matrix
    PrepareRendering(); // prepare to render.
    RenderContent(); // render
    glPopMatrix(); // restore the saved matrix.
}

VisualObject::GLNameType VisualObject::CreateName()
{
    static GLNameType Name = 0x88;
    Name++;
    return Name;
}

VisualObject * VisualObject::GetObjectByGLName(GLNameType name)
{
    if (Name_ == name)
        return this;
    else
        return nullptr;
}
