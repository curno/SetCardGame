#include "Include/stdafx.h"
#include "Include/Set.h"
#include "Include/UI/GameView.h"
#include "Include/Rendering/VisualObject.h"
#include "Include/Rendering/Geometry.h"
#include "Include/Animation/AnimationManager.h"

#define LARGE_SCALE 2000

// MainView
GameView::GameView()
{
    Game_ = ref<Game>(new Game);
    GameScene_ = ref<VisualGameScene>(new VisualGameScene(Game_));
}

GameView::~GameView()
{
}

IMPLEMENT_DYNCREATE(GameView, OpenGLView)


void GameView::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);

    if (GameScene_ != nullptr)
        GameScene_->Size = Dimension(cx, cy, LARGE_SCALE);
    Invalidate(NULL);
}

void GameView::RenderWithOpenGL()
{
    if (GameScene_ != nullptr)
    {
        glMatrixMode(GL_PROJECTION);
        glOrtho(0, GameScene_->Size.Width, 0, GameScene_->Size.Height, -GameScene_->Size.Depth, GameScene_->Size.Depth);
        glMatrixMode(GL_MODELVIEW);

        // render
        GameScene_->Render();
    }
}

void GameView::InitOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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

}

void GameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

    __super::OnKeyDown(nChar, nRepCnt, nFlags);
    GameScene_->Hint();

}

VisualObject *GameView::GetObjectByGLName(VisualObject::GLNameType name)
{
    return GameScene_->GetObjectByGLName(name);
}
