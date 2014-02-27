#pragma once

#include "VisualScene.h"
#include "VisualButton.h"
#include "VisualGameScene.h"
#include "../../Res/resource.h"

class VisualPanel : public VisualScene
{
    ref<VisualButton> ButtonNewGame_;
    ref<VisualButton> ButtonCommit_;
    VisualGameScene *GameScene_;
protected:
    virtual void RenderContent() override
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_PANEL_BACKGROUND));
        double w = Size.Width;
        double h = Size.Height;
        // first render panel.
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0);
        glVertex2d(0.0, 0.0);
        glTexCoord2d(1.0, 0.0);
        glVertex2d(w, 0.0);
        glTexCoord2d(1.0, 1.0);
        glVertex2d(w, h);
        glTexCoord2d(0.0, 1.0);
        glVertex2d(0, h);
        glEnd();

        __super::RenderContent();
    }

    virtual void PrepareRendering()
    {

    }

    virtual void OnResize(const CSize &size)
    {
        double margin = (size.cx - (2 * ButtonWidth)) / 3.0;
        double button_height = ButtonWidth * VisualButton::HeightPerWidth;

        ButtonNewGame_->Size = Dimension(ButtonWidth, static_cast<Coordinate>(button_height), 0);
        ButtonCommit_->Size = Dimension(ButtonWidth, static_cast<Coordinate>(button_height), 0);

        ButtonNewGame_->Position = Point(static_cast<Coordinate>(margin + ButtonWidth / 2.0),
            static_cast<int>(BottomMargin + button_height / 2.0), 1);
        ButtonCommit_->Position = Point(static_cast<Coordinate>(2 * margin + 3 * ButtonWidth / 2.0),
            static_cast<int>(BottomMargin + button_height / 2.0), 1);
    }

public:
    VisualPanel(VisualGameScene *scene) : GameScene_(scene)
    {
        Material_ = Material::GetMaterial("default");
        ButtonNewGame_ = ::std::make_shared<VisualButton>(MakeGenericOperation([this](){ GameScene_->Start(); }));
        ButtonNewGame_->SetTextureName(IDB_NEW_GAME);

        ButtonCommit_ = ::std::make_shared<VisualButton>(MakeGenericOperation([](){}));
        ButtonCommit_->SetTextureName(IDB_COMMIT);
        AddChild(ButtonNewGame_);
        AddChild(ButtonCommit_);
    }

private:
    static const int ButtonWidth = 100;
    static const int BottomMargin = 20;
};