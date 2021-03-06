#include "Include/stdafx.h"

#include "Include/Rendering/VisualDeck.h"

void VisualDeck::OnMouseButtonDown()
{
    __super::OnMouseButtonDown();
    if (!Enabled)
        return;
    if (Operation_ != nullptr)
        (*Operation_)();
}

void VisualDeck::RenderContent()
{
    __super::RenderContent();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    const double factor = VisualCard::DepthPerWidthRatio / (2 * VisualCard::DepthPerWidthRatio + 1);

    const double length = Size.Depth;
    const double mfactor = 1 - factor;

    Point p0(-Size.Width / 2, -Size.Height / 2, -Size.Depth / 2);
    Point p1 = -p0;

    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_CARD_COVERED));
    // front
    glBegin(GL_QUADS);
    glNormal3d(0.0, 0.0, 1.0);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(p0.X, p0.Y, p1.Z);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(p1.X, p0.Y, p1.Z);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(p1.X, p1.Y, p1.Z);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(p0.X, p1.Y, p1.Z);
    glEnd();
#pragma region Draw other five faces.
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_CARD_BACKGROUND));
    glBegin(GL_QUADS);
    // top
    glNormal3d(0.0, 1.0, 0.0);
    glTexCoord2d(mfactor, 1.0);
    glVertex3d(p1.X, p1.Y, p1.Z);
    glTexCoord2d(mfactor, mfactor);
    glVertex3d(p1.X, p1.Y, -p1.Z);
    glTexCoord2d(factor, mfactor);
    glVertex3d(p0.X, p1.Y, -p1.Z);
    glTexCoord2d(factor, 1.0);
    glVertex3d(p0.X, p1.Y, p1.Z);

    // bottom
    glNormal3d(0.0, -1.0, 0.0);
    glTexCoord2d(mfactor, 0.0);
    glVertex3d(p1.X, p0.Y, p1.Z);
    glTexCoord2d(factor, 0.0);
    glVertex3d(p0.X, p0.Y, p1.Z);
    glTexCoord2d(factor, factor);
    glVertex3d(p0.X, p0.Y, -p1.Z);
    glTexCoord2d(mfactor, factor);
    glVertex3d(p1.X, p0.Y, -p1.Z);

    // left
    glNormal3d(-1.0, 0.0, 0.0);
    glTexCoord2d(0, factor);
    glVertex3d(p0.X, p0.Y, p1.Z);
    glTexCoord2d(0, mfactor);
    glVertex3d(p0.X, p1.Y, p1.Z);
    glTexCoord2d(factor, mfactor);
    glVertex3d(p0.X, p1.Y, -p1.Z);
    glTexCoord2d(factor, factor);
    glVertex3d(p0.X, p0.Y, -p1.Z);

    // right
    glNormal3d(1.0, 0.0, 0.0);
    glTexCoord2d(1, factor);
    glVertex3d(p1.X, p0.Y, p1.Z);
    glTexCoord2d(mfactor, factor);
    glVertex3d(p1.X, p0.Y, -p1.Z);
    glTexCoord2d(mfactor, mfactor);
    glVertex3d(p1.X, p1.Y, -p1.Z);
    glTexCoord2d(1.0, mfactor);
    glVertex3d(p1.X, p1.Y, p1.Z);
    glEnd();

      // back
    glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_CARD_COVERED));
    glBegin(GL_QUADS);
    glNormal3d(0.0, 0.0, -1.0);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(p0.X, p0.Y, -p1.Z);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(p0.X, p1.Y, -p1.Z);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(p1.X, p1.Y, -p1.Z);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(p1.X, p0.Y, -p1.Z);
    glEnd();
#pragma endregion

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}

void VisualDeck::OnMouseEnter()
{
    __super::OnMouseEnter();
    static const int HoverAnimationDuration = 500;
    static const double RotateTheta = -0.5;
    BlinkAnimation_ = nullptr;

    Transformation transformation;
    transformation.RotateByCenter(-Size.Width / 2, Size.Height / 2, 0.0, 0.0, 0.0, 1.0, RotateTheta);
    HoverAnimation_ = MakeGenericAnimation(HoverAnimationDuration, ::Transform(GetTransformation(), transformation));
    HoverAnimation_->Start();
}

void VisualDeck::Highlight(Material &m)
{
    GLfloat emission[] = { EmissionHighlight_, EmissionHighlight_, EmissionHighlight_, 1.0f };
    m.SetData(Material::Parameter::Emission, emission);
}

void VisualDeck::OnMouseLeave()
{
    __super::OnMouseLeave();
    static const int HoverAnimationDuration = 500;
    BlinkAnimation_ = nullptr;

    HoverAnimation_ = MakeGenericAnimation(HoverAnimationDuration, ::Transform(GetTransformation(), Transformation()));
    HoverAnimation_->Start();
}

void VisualDeck::SetNormalEmission(Material &m)
{
    GLfloat emission[] = { EmissionNormal_, EmissionNormal_, EmissionNormal_, 1.0f };
    m.SetData(Material::Parameter::Emission, emission);
}

void VisualDeck::Blink()
{
    static const int duration = 1000;
    Material material = Material_;
    SetNormalEmission(material);
    Material material_highlight = Material_;
    Highlight(material_highlight);
    BlinkAnimation_ = MakeGenericAnimation(1000, ::Blink(Material_, material, material_highlight));
    BlinkAnimation_->Start();
}

