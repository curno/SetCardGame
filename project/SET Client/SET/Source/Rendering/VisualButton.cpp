#include "Include/stdafx.h"

#include "Include/Rendering/VisualButton.h"
#include "Include/Rendering/TextureManager.h"

void VisualButton::RenderContent()
{
    __super::RenderContent();

    Dimension size = Size;
    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(TextureName_));
    glBegin(GL_POLYGON);
    glNormal3d(0.0, 0.0, 1.0);
    glTexCoord2d(CornerRatioX, 1.0);
    glVertex2d(-size.Width / 2 + size.Width * CornerRatioX, size.Height / 2);
    glTexCoord2d(1.0 - CornerRatioX, 1.0);
    glVertex2d(size.Width / 2 - size.Width * CornerRatioX, size.Height / 2);
    glTexCoord2d(1.0, 1.0 - CornerRatioY);
    glVertex2d(size.Width / 2, size.Height / 2 - size.Height * CornerRatioY);
    glTexCoord2d(1.0, CornerRatioY);
    glVertex2d(size.Width / 2, -size.Height / 2 + size.Height * CornerRatioY);
    glTexCoord2d(1.0 - CornerRatioX, 0.0);
    glVertex2d(size.Width / 2 - size.Width * CornerRatioX, -size.Height / 2);
    glTexCoord2d(CornerRatioX, 0.0);
    glVertex2d(-size.Width / 2 + size.Width * CornerRatioX, -size.Height / 2);
    glTexCoord2d(0.0, CornerRatioY);
    glVertex2d(-size.Width / 2, -size.Height / 2 + size.Height * CornerRatioY);
    glTexCoord2d(0.0, 1.0 - CornerRatioY);
    glVertex2d(-size.Width / 2, size.Height / 2 - size.Height * CornerRatioY);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
}

void VisualButton::PrepareRendering()
{
    __super::PrepareRendering();

    // enable light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // light parameter
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.02f };
    GLfloat light_ambient[] = { 0.6f, 0.6f, 0.6f, 0.8f };
    GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 0.6f };
    GLfloat light_specular[] = { 0.4f, 0.4f, 0.4f, 0.6f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

const double VisualButton::CornerRatioX = 0.1;
const double VisualButton::CornerRatioY = 0.1;
const double VisualButton::HeightPerWidthRatio = 0.42;
const double VisualButton::DepthPerWidthRatio = 0.2;
