#include "Include/stdafx.h"

#include "Res/resource.h"
#include "Include/Rendering/VisualCard.h"
#include "Include/Rendering/VisualGameScene.h"
#include "Include/Rendering/TextureManager.h"
#include "Include/Animation/VisualObjectAnimations.h"
#include "Res/resource.h"

void VisualCard::RenderContent()
{
    const double factor = DepthPerWidthRatio / (2 * DepthPerWidthRatio + 1);

    const double length = Size.Depth;
    const double mfactor = 1 - factor;

    Point p0(-Size.Width / 2, -Size.Height / 2, -Size.Depth / 2);
    Point p1 = -p0;

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_TEST));
    glBegin(GL_TRIANGLE_STRIP);
    // front
    glNormal3d(0.0, 0.0, 1.0);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(p0.X, p0.Y, p1.Z);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(p1.X, p0.Y, p1.Z);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(p0.X, p1.Y, p1.Z);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(p1.X, p1.Y, p1.Z);
    glEnd();

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

    // back
    glNormal3d(0.0, 0.0, -1.0);
    glTexCoord2d(factor, factor);
    glVertex3d(p0.X, p0.Y, -p1.Z);
    glTexCoord2d(factor, mfactor);
    glVertex3d(p0.X, p1.Y, -p1.Z);
    glTexCoord2d(mfactor, mfactor);
    glVertex3d(p1.X, p1.Y, -p1.Z);
    glTexCoord2d(mfactor, factor);
    glVertex3d(p1.X, p0.Y, -p1.Z);
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
    GLfloat emission[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    Material_.SetData(Material::Parameter::Emission, emission);
    
}

void VisualCard::OnMouseLeave()
{
    GLfloat emission[] = { 0.0, 0.0, 0.0, 0.0 };
    Material_.SetData(Material::Parameter::Emission, emission);
    
}

VisualCard::VisualCard(const CardRef card, VisualGameScene *parent) : Card_(card), State_(State::OnDesk), Parent_(parent)
{
    Material_ = (Material::GetMaterial("default"));
}

void VisualCard::OnMouseButtonDown()
{
    __super::OnMouseButtonDown();
    switch (CurrentState)
    {
    case VisualCard::State::OnDesk:
        Choosed();
        Parent_->OnCardChoosed(SHARED_THIS);
        break;
    case VisualCard::State::Choosed:
        CancelChoosed();
        Parent_->OnCardCancleChoosed(SHARED_THIS);
        break;
    case VisualCard::State::Discarded:
        // do nothing
        break;
    default:
        break;
    }
}

void VisualCard::CancelChoosed()
{
    if (this->GetIsChoosed())
    {
        CurrentState = State::OnDesk;
        if (Animation_ != nullptr)
            Animation_->Stop();
        Transformation target;
        Animation_ = MakeGenericAnimation(300, ::Transform(this->GetTransformation(), Transformation()));
        Animation_->Start();
    }
}

void VisualCard::Choosed()
{
    if (!this->GetIsChoosed())
    {
        CurrentState = State::Choosed;
        if (Animation_ != nullptr)
            Animation_->Stop();
        double theta = VisualGameScene::SlopeTheta;

        Transformation target;
        target.Translate(0.0, Size.Height / 2.0, Size.Width / 2.0);
        target.Scale(1.1, 1.1, 1.1);
        target.Rotate(1.0, 0.0, 0.0, theta);
        target.Translate(0.0, -Size.Height / 2.0, 0.0);
        Animation_ = MakeGenericAnimation(300, ::Transform(this->GetTransformation(), target));
        Animation_->Start();
    }
}

void VisualCard::Discarded()
{
    if (CurrentState != State::Discarded)
        CurrentState = State::Discarded;
}



double VisualCard::HeightPerWidthRatio = 1.618; // 1 : 0.618
double VisualCard::DepthPerWidthRatio = 0.25; // 1 : 3