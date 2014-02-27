#pragma once

#include "stdafx.h"
#include "OpenGLView.h"

// MainView 窗口

class PanelView : public OpenGLView
{
    DECLARE_DYNCREATE(PanelView)
private:
    // 构造
public:
    PanelView() { }


    // 重写
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    // 实现
public:
    virtual ~PanelView() { }
};

