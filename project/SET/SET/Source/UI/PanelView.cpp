#include "Include/stdafx.h"

#include "Include/UI/PanelView.h"
#include "Include/UI/MainView.h"

IMPLEMENT_DYNCREATE(PanelView, OpenGLView)

BOOL PanelView::PreCreateWindow(CREATESTRUCT& cs)
{
    return __super::PreCreateWindow(cs);
}

void PanelView::RenderWithOpenGL()
{
    RECT rect;
    GetClientRect(&rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, w, 0, h, -100, 100);
    glMatrixMode(GL_MODELVIEW);

    Panel_->Render();
}

VisualObject *PanelView::GetObjectByGLName(VisualObject::GLNameType name)
{
    return Panel_->GetObjectByGLName(name);
}

void PanelView::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);
    Panel_->Size = Dimension(cx, cy, 100);
    Invalidate(NULL);
}

PanelView::PanelView(VisualGameScene *scene) 
{
    Panel_ = ::std::make_shared<VisualPanel>(scene);
}
