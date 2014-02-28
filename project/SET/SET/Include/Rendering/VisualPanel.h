#pragma once

#include "VisualScene.h"
#include "VisualDeck.h"
#include "VisualButton.h"
#include "VisualGameScene.h"
#include "../../Res/resource.h"

class VisualPanel : public VisualScene
{
    ref<VisualButton> ButtonNewGame_;
    ref<VisualButton> ButtonCommit_;
    ref<VisualButton> ButtonHint_;
    ref<VisualDeck> Deck_;
    VisualGameScene *GameScene_; // the scene this panel will control.
protected:
    virtual void RenderContent() override
    {
       // glDisable(GL_DEPTH_TEST);
        CTimeSpan span = GameScene_->GetGameElapsedTime();
        char formated_time[256];
        sprintf_s(formated_time, "%d:%d", span.GetMinutes(), span.GetSeconds());
        glColor3d(1.0, 0.0, 0.0);
        //TextRenderer::Instance().DrawTextMy(formated_time, Point(0, 0, 1000));
        glColor3d(1.0, 1.0, 1.0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_PANEL_BACKGROUND));
        double w = Size.Width;
        double h = Size.Height;
        // first render panel.
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0);
        glVertex2d(-w / 2.0, -h / 2.0);
        glTexCoord2d(1.0, 0.0);
        glVertex2d(w / 2.0, -h / 2.0);
        glTexCoord2d(1.0, 1.0);
        glVertex2d(w / 2.0, h / 2.0);
        glTexCoord2d(0.0, 1.0);
        glVertex2d(-w / 2.0, h / 2.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        __super::RenderContent();

        GLdouble mv[16];
        GLdouble pj[16];
        GLint vp[4];
        glGetDoublev(GL_MODELVIEW_MATRIX, mv);
        glGetDoublev(GL_PROJECTION_MATRIX, pj);
        glGetIntegerv(GL_VIEWPORT, vp);
        double x, y, z;
        gluProject(0.0, 0.0,600, mv, pj, vp, &x, &y, &z);

    }

    virtual void PrepareRendering()
    {
        __super::PrepareRendering();
    }

    virtual void OnResize(const CSize &size)
    {
        double button_height = ButtonWidth * VisualButton::HeightPerWidthRatio;
        double deck_width = size.cy - 2 * Margin;
        double deck_height = deck_width * VisualCard::HeightPerWidthRatio;
        Deck_->Size = Dimension(static_cast<Coordinate>(deck_width),
            static_cast<Coordinate>(deck_height),
            static_cast<Coordinate>(VisualCard::DepthPerWidthRatio * deck_width));
        Deck_->Position = Point(Margin - Size.Width / 2 + Deck_->Size.Height / 2, 0, Deck_->Size.Depth / 2);
        GetTransformation(IndexOf(Deck_)).Rotate(0.0, 0.0, 1.0, PI / 4);

        Dimension button_size = Dimension(ButtonWidth,
            static_cast<Coordinate>(ButtonWidth * VisualButton::HeightPerWidthRatio),
            static_cast<Coordinate>(ButtonWidth * VisualButton::DepthPerWidthRatio)
            );
        ButtonNewGame_->Size = button_size;
        ButtonHint_->Size = button_size;
        ButtonCommit_->Size = button_size;

        double margin = (Deck_->Size.Height - 3 * button_height) / 2.0;
        ButtonNewGame_->Position = Point(static_cast<Coordinate>(size.cx / 2 - Margin * 3 - ButtonWidth * 2.5),
            0,
            button_size.Depth / 2);
        ButtonCommit_->Position = Point(static_cast<Coordinate>(size.cx / 2 - Margin - ButtonWidth * 0.5),
            0,
            button_size.Depth / 2);
        ButtonHint_->Position = Point(static_cast<Coordinate>(size.cx / 2 - Margin * 2 - ButtonWidth * 1.5),
            0,
            button_size.Depth / 2);
    }

public:
    VisualPanel(VisualGameScene *scene) : GameScene_(scene)
    {
        Material_ = Material::GetMaterial("default");
        ButtonNewGame_ = ::std::make_shared<VisualButton>(MakeGenericOperation([this](){ GameScene_->Start(); }));
        ButtonNewGame_->SetTextureName(IDB_NEW_GAME);
        ButtonHint_ = ::std::make_shared<VisualButton>(MakeGenericOperation([this]() { GameScene_->Hint(); }));
        ButtonHint_->SetTextureName(IDB_HINT);
        ButtonCommit_ = ::std::make_shared<VisualButton>(MakeGenericOperation([](){}));
        ButtonCommit_->SetTextureName(IDB_COMMIT);
        Deck_ = ::std::make_shared<VisualDeck>();
        Deck_->Operation = MakeGenericOperation([this](){GameScene_->Deal(); });
        AddChild(ButtonNewGame_);
        AddChild(ButtonHint_);
        AddChild(ButtonCommit_);
        AddChild(Deck_);
    }

private:
    static const int ButtonWidth = 80;
    static const int Margin = 15;
    
};