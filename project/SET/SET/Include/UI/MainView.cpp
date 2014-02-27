#include "Include/stdafx.h"
#include "MainView.h"
#include "Include/Animation/AnimationManager.h"

IMPLEMENT_DYNCREATE(MainView, CView)
BEGIN_MESSAGE_MAP(MainView, CView)
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_WM_TIMER()
END_MESSAGE_MAP()


void MainView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
    GameView_->MoveWindow(0, 0, cx - PanelWidth, cy);
    PanelView_->MoveWindow(cx - PanelWidth, 0, PanelWidth, cy);
}

MainView::MainView()
{
    GameView_ = new GameView();
    PanelView_ = new PanelView();
}


int MainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    if (!GameView_->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 800, 400), this, AFX_IDW_PANE_FIRST, NULL))
    {
        TRACE0("未能创建视图窗口\n");
        return -1;
    }
    
    if (!PanelView_->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 800, 400), this, AFX_IDW_PANE_FIRST + 1, NULL))
    {
        TRACE0("未能创建视图窗口\n");
        return -1;
    }

    SetTimer(1, 20, NULL);
    return 0;
}


void MainView::OnTimer(UINT_PTR nIDEvent)
{
    CView::OnTimer(nIDEvent);
    AnimationManager::Instance().PerformAllAnimation(); // perform animation
    Invalidate(NULL); // always refresh. 
}
