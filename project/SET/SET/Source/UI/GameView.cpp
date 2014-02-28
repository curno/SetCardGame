#include "Include/stdafx.h"
#include "Include/Set.h"
#include "Include/UI/GameView.h"
#include "Include/Rendering/VisualObject.h"
#include "Include/Utils/Geometry.h"
#include "Include/Animation/AnimationManager.h"

#define LARGE_SCALE 2000

// MainView
GameView::GameView()
{
    Game_ = ::std::make_shared<Game>();
    GameScene_ = ::std::make_shared<VisualGameScene>(Game_);
    ControlPanel_ = ::std::make_shared<VisualPanel>(GameScene_.get());
}

GameView::~GameView()
{
}

IMPLEMENT_DYNCREATE(GameView, OpenGLView)


void GameView::OnSize(UINT nType, int cx, int cy)
{
    static const int ControlPanelHeight = 100;
    static const int ControlPanelDepth = 500;
    static const double ControlPanelWidthRatio = 0.8;
    __super::OnSize(nType, cx, cy);

    if (GameScene_ != nullptr)
    {
        GameScene_->Size = Dimension(cx, cy - ControlPanelHeight, LARGE_SCALE);
        GameScene_->Position = Point(cx / 2, ControlPanelHeight + GameScene_->Size.Height / 2, 0);
    }
    if (ControlPanel_ != nullptr)
    {
        ControlPanel_->Size = Dimension(static_cast<Coordinate>(cx * ControlPanelWidthRatio),
            ControlPanelHeight, ControlPanelDepth);
        ControlPanel_->Position = Point(cx / 2, ControlPanel_->Size.Height / 2, ControlPanel_->Size.Depth);
    }
    Invalidate(NULL);
}

void GameView::RenderWithOpenGL()
{
    if (GameScene_ != nullptr)
    {
        RECT rect;
        CWnd::GetClientRect(&rect);
        glMatrixMode(GL_PROJECTION);
        glOrtho(0, rect.right - rect.left, 0, rect.bottom - rect.top, -LARGE_SCALE, LARGE_SCALE);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // render
        glPushMatrix();
        glTranslated(GameScene_->Position.X, GameScene_->Position.Y, GameScene_->Position.Z);
        GameScene_->Render();
        auto a = gluErrorString(glGetError());
        glPopMatrix();


        glPushMatrix();
        glTranslated(ControlPanel_->Position.X, ControlPanel_->Position.Y, ControlPanel_->Position.Z);
        ControlPanel_->Render();
        glPopMatrix();
    }
}


void GameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    __super::OnKeyDown(nChar, nRepCnt, nFlags);
    GameScene_->Hint();
}

VisualObject *GameView::GetObjectByGLName(VisualObject::GLNameType name)
{
    VisualObject *object = ControlPanel_->GetObjectByGLName(name);
    if (object != nullptr)
        return object;
    object = GameScene_->GetObjectByGLName(name);
    if (object != nullptr)
        return object;
    return nullptr;
}

void GameView::InitOpenGL()
{
    __super::InitOpenGL();
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
