#include "Include/stdafx.h"

#include "Include/Rendering/VisualObject.h"


void VisualObject::Render()
{
    glLoadName(Name_);
    glMatrixMode(GL_MODELVIEW); // Select ModelView matrix.
    glPushMatrix(); // save current matrix
    glMultMatrixd(Transformation_.Data); // add current transform to current matrix
    GLdouble mv[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, mv);
    PrepareRendering(); // prepare to render.
    RenderContent(); // render
    glPopMatrix(); // restore the saved matrix.
}

VisualObject::GLNameType VisualObject::CreateName()
{
    static GLNameType Name = 0x21; // beginning name
    Name++;
    return Name;
}

VisualObject *VisualObject::GetObjectByGLName(GLNameType name)
{
    if (Name_ == name) // if self has the name, return this.
        return this;
    else
        return nullptr;
}

void VisualObject::SetMaterial()
{
    // set material.
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Material_.Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Material_.Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Material_.Specular);
    GLfloat shininess = Material_.Shininess[0] * 255;
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Material_.Emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
}

void VisualObject::PrepareRendering()
{
    SetMaterial();
}
