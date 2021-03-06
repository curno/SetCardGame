#include "Include/stdafx.h"

#include "Res/resource.h"
#include "Include/Rendering/VisualCard.h"
#include "Include/Rendering/VisualGameScene.h"
#include "Include/Rendering/TextureManager.h"
#include "Include/Animation/VisualObjectAnimations.h"
#include "Res/resource.h"

void VisualCard::RenderContent()
{
    __super::RenderContent();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    const double factor = DepthPerWidthRatio / (2 * DepthPerWidthRatio + 1);

    const double length = Size.Depth;
    const double mfactor = 1 - factor;

    Point p0(-Size.Width / 2, -Size.Height / 2, -Size.Depth / 2);
    Point p1 = -p0;

    glDisable(GL_BLEND);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(Card_));
    
    int number = Card_->GetIntegerNumber();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(0.0, 0.0, p1.Z);
    double symbol_width = SymbolRatio * Size.Height;
    double margin = (Size.Height - symbol_width * number) / (number + 1);
    // segment 1
    glDisable(GL_TEXTURE_2D);
    RenderFrontRectangle(p0.X, p0.Y, p1.X, p0.Y + margin);
    double current_y = p0.Y + margin;
    for (int i = 0; i < number; ++i)
    {
        // segment 2
        glEnable(GL_TEXTURE_2D);
        RenderFrontRectangle(p0.X, current_y, p1.X, current_y + symbol_width);
        current_y += symbol_width;
        // segment 3
        glDisable(GL_TEXTURE_2D);
        RenderFrontRectangle(p0.X, current_y, p1.X, current_y + margin);
        current_y += margin;
    }
    glPopMatrix();
    
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

    // back
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

VisualCard::VisualCard(const CardRef card, VisualGameScene *parent) : VisualWidget("default"), Card_(card), State_(State::OnDesk), Parent_(parent) 
{
    EmissionNormal_ = 0.0;
    UpdateMaterialEmission();
}

void VisualCard::OnMouseButtonDown()
{
    __super::OnMouseButtonDown();
    if (!Enabled)
        return;
    switch (CurrentState)
    {
    case VisualCard::State::OnDesk:
        Choose();
        Parent_->OnCardChosen(SHARED_THIS);
        break;
    case VisualCard::State::Chosen:
        CancelChosen();
        Parent_->OnCardCancleChosen(SHARED_THIS);
        break;
    case VisualCard::State::Discarded:
    default:
        break;
    }
}

void VisualCard::CancelChosen()
{
    if (this->GetIsChosen())
    {
        CurrentState = State::OnDesk;
        
        CancelChosenAnimate();
    }
}

void VisualCard::Choose()
{
    if (!this->GetIsChosen())
    {
        CurrentState = State::Chosen;
        
        ChosenAnimate();
    }
}

void VisualCard::Discarded()
{
    if (CurrentState != State::Discarded)
        CurrentState = State::Discarded;
}

void VisualCard::RenderFrontRectangle(GLdouble p0x, GLdouble p0y, GLdouble p1x, GLdouble p1y)
{
    glBegin(GL_QUADS);
    glNormal3d(0.0, 0.0, 1.0);
    glTexCoord2d(0.0, 0.0);
    glVertex2d(p0x, p0y);
    glTexCoord2d(0.0, 1.0);
    glVertex2d(p1x, p0y);
    glTexCoord2d(1.0, 1.0);
    glVertex2d(p1x, p1y);
    glTexCoord2d(1.0, 0.0);
    glVertex2d(p0x, p1y);
    glEnd();
}

void VisualCard::PrepareRendering()
{
    __super::PrepareRendering();
    glMatrixMode(GL_MODELVIEW);
    glMultMatrixd(ShakeAnimationTransformation_.Data); // apply shake.
}

void VisualCard::Shake()
{
    ShakeAnimation_ = MakeGenericAnimation(1000, ::Shake(ShakeAnimationTransformation_, 0.0, 0.0, 1.0));
    ShakeAnimation_->Start();
}

void VisualCard::ChosenAnimate()
{
    double theta = VisualGameScene::SlopeTheta;
    Transformation target;
    target.Translate(0.0, Size.Height / 2.0, Size.Width / 2.0);
    target.Scale(1.1, 1.1, 1.1);
    target.Rotate(1.0, 0.0, 0.0, theta);
    target.Translate(0.0, -Size.Height / 2.0, 0.0);
    ChosenAnimation_ = MakeGenericAnimation(300, ::Transform(this->GetTransformation(), target));
    ChosenAnimation_->Start();
}

void VisualCard::CancelChosenAnimate()
{
    Transformation target;
    ChosenAnimation_ = MakeGenericAnimation(300, ::Transform(this->GetTransformation(), Transformation()));
    ChosenAnimation_->Start();
}

double VisualCard::HeightPerWidthRatio = 1.618; // 1 : 0.618
double VisualCard::DepthPerWidthRatio = 0.25; // 1 : 3
double VisualCard::SymbolRatio = 0.25; // 1 : 3