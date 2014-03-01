#include "Include/stdafx.h"

#include "Include/Rendering/MainScene.h"


MainScene::MainScene()
{
    Game_ = ::std::make_shared<Game>();
    GameScene_ = ::std::make_shared<VisualGameScene>(Game_);
    ControlPanel_ = ::std::make_shared<VisualPanel>(GameScene_.get());

    AddChild(GameScene_);
    AddChild(ControlPanel_);
}

void MainScene::OnResize(const CSize &size)
{
    static const int ControlPanelHeight = 100;
    static const int ControlPanelDepth = 500;
    static const double ControlPanelWidthRatio = 0.8;
    static const int LargeScale = 2000;
    __super::OnResize(size);
    GameScene_->Size = Dimension(size.cx, size.cy - ControlPanelHeight, LargeScale);
    GameScene_->Position = Point(size.cx / 2, GameScene_->Size.Height / 2, 0);

    ControlPanel_->Size = Dimension(static_cast<Coordinate>(size.cx * ControlPanelWidthRatio),
        ControlPanelHeight, ControlPanelDepth);
    ControlPanel_->Position = Point(size.cx / 2, ControlPanel_->Size.Height / 2 + GameScene_->Size.Height, ControlPanel_->Size.Depth);
    GameScene_->SetDealCardStartPosition(Point(-ControlPanel_->Size.Width / 2 + 20, ControlPanel_->Position.Y - GameScene_->Position.Y, 0));
}

void MainScene::PrepareRendering()
{
    // enable light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // light parameter
    GLfloat light_position[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    GLfloat light_ambient[] = { 0.6f, 0.6f, 0.6f, 0.6f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 0.6f };
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
