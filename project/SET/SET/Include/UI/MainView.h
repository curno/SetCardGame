#pragma once

#include "stdafx.h"
#include "GameView.h"
#include "PanelView.h"

class MainView : public CView
{
    DECLARE_DYNCREATE(MainView)
    GameView *GameView_;
    PanelView *PanelView_;
public:
    MainView();
public:
    virtual ~MainView() { }
    virtual void OnDraw(CDC* pDC) override { }
protected:
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
    static const int PanelWidth = 300;
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};