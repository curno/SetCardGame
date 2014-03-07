#include "Include/stdafx.h"

#include "Include/Rendering/VisualObject.h"

void VisualObject::Render()
{
    // render only when visible.
    if (Visible)
    {
        glMatrixMode(GL_MODELVIEW); // Select ModelView matrix.
        glPushMatrix(); // save current matrix
        glMultMatrixd(Transformation_.Data); // add current transform to current matrix
        PrepareRendering(); // prepare to render.
        RenderContent(); // render
        glPopMatrix(); // restore the saved matrix.
    }
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

