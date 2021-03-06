#include "Include/stdafx.h"
#include "Include/Set.h"
#include "Include/UI/MainView.h"
#include "Include/Rendering/VisualObject.h"
#include "Include/Utils/Geometry.h"
#include "Include/Animation/AnimationManager.h"
#include "Include/Rendering/VisualGameScene.h"
#include "Include/Rendering/VisualPanel.h"
#include "Include/Animation/GenericAnimation.h"
#include "Include/Animation/VisualObjectAnimations.h"

// This is the z direction length of OPENGL projection cube.
// It should be as large as possible that the cube can hold any object inside it.
// For now, 2000 is OK.
#define LARGE_SCALE 2000

// MainView
MainView::MainView()
{
    UI_ = ::std::make_shared<MainScene>();
}

MainView::~MainView()
{
}

IMPLEMENT_DYNCREATE(MainView, OpenGLView)

void MainView::OnSize(UINT nType, int cx, int cy)
{
    
    __super::OnSize(nType, cx, cy);

    if (UI_ != nullptr)
    {
        UI_->Position = Point();
        UI_->Size = Dimension(cx, cy , LARGE_SCALE);
    }
    Invalidate(NULL);
}

void MainView::RenderWithOpenGL()
{
    if (UI_ != nullptr)
    {
        RECT rect;
        CWnd::GetClientRect(&rect);
        glMatrixMode(GL_PROJECTION);
        glOrtho(0, rect.right - rect.left, 0, rect.bottom - rect.top, -LARGE_SCALE, LARGE_SCALE);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // render ui
        UI_->Render();
    }
}

void MainView::PickingWithOpenGL()
{
    if (UI_ != nullptr)
    {
        RECT rect;
        CWnd::GetClientRect(&rect);
        glMatrixMode(GL_PROJECTION);
        glOrtho(0, rect.right - rect.left, 0, rect.bottom - rect.top, -LARGE_SCALE, LARGE_SCALE);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // render ui
        UI_->RenderForPicking();
    }
}

VisualWidget *MainView::GetWidgetByGLName(VisualObject::GLNameType name)
{
    if (UI_ != nullptr)
        return UI_->GetWidgetByGLName(name);
    return nullptr;
}

VisualWidget *MainView::GetWidgetByViewportPosition(CPoint position)
{
    if (UI_ != nullptr)
        return UI_->GetWidgetByViewportPosition(position);
    return nullptr;
}

