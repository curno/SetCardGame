#include "Include/stdafx.h"

#include "Res/resource.h"
#include "Include/Rendering/VisualCard.h"

void VisualCard::RenderContent()
{
    glEnable(GL_TEXTURE_2D);
    double length = Size.cx / 2.0;
    glBegin(GL_TRIANGLE_STRIP);

    // front
    glNormal3d(0.0, 0.0, 1.0);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(0, 0, length);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(Size.cx, 0, length);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(0, Size.cy, length);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(Size.cx, Size.cy, length);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    // top
    glNormal3d(0.0, 1.0, 0.0);
    glVertex3d(Size.cx, Size.cy, length);
    glVertex3d(Size.cx, Size.cy, 0.0);
    glVertex3d(0.0, Size.cy, 0.0);
    glVertex3d(0.0, Size.cy, length);

    // bottom
    glNormal3d(0.0, -1.0, 0.0);
    glVertex3d(Size.cx, 0.0, length);
    glVertex3d(0.0, 0.0, length);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(Size.cx, 0.0, 0.0);

    // left
    glNormal3d(-1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, length);
    glVertex3d(0.0, Size.cy, length);
    glVertex3d(0.0, Size.cy, 0.0);
    glVertex3d(0.0, 0.0, 0.0);

    // right
    glNormal3d(1.0, 0.0, 0.0);
    glVertex3d(Size.cx, 0.0, length);
    glVertex3d(Size.cx, 0.0, 0.0);
    glVertex3d(Size.cx, Size.cy, 0.0);
    glVertex3d(Size.cx, Size.cy, length);

    // back
    glNormal3d(0.0, 0.0, -1.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, Size.cy, 0.0);
    glVertex3d(Size.cx, Size.cy, 0.0);
    glVertex3d(Size.cx, 0.0, 0.0);
    glEnd();
    
    /*GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    double x, y, z;
    gluProject(0, 0, 0, modelview, projection, viewport, &x, &y, &z);*/
}

void VisualCard::PrepareRendering()
{
    if (TexName_ == 0) // if no texture object, create.
        GenTexture();

    // set material.
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Material_.Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Material_.Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Material_.Specular);
    GLfloat shininess = Material_.Shininess[0] * 255;
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Material_.Emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TexName_);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
}

void VisualCard::OnMouseMove()
{
    __super::OnMouseMove();
    if (true)
    {
       // Rotate(0.0, 1.0, 0.0, -0.3);
    }
}

void VisualCard::OnMouseEnter()
{
    //Rotate(0.0, 1.0, 0.0, -0.3);
    GLfloat emission[] = { 0.8f, 0.2f, 0.2f, 1.0f };
    Material_.SetData(Material::Parameter::Emission, emission);
}

void VisualCard::OnMouseLeave()
{
    //Rotate(0.0, 1.0, 0.0, 0.3);
    GLfloat emission[] = { 0.0, 0.0, 0.0, 0.0 };
    Material_.SetData(Material::Parameter::Emission, emission);
}


