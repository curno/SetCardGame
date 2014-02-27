#pragma once

#include "stdafx.h"
#include "OpenGLView.h"
#include "../Rendering/VisualPanel.h"

// MainView 窗口
class MainView;
class PanelView : public OpenGLView
{
    DECLARE_DYNCREATE(PanelView)
    ref<VisualPanel> Panel_;
private:
    // 构造
public:
    PanelView(VisualGameScene *scene = nullptr);

    // 重写
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;
    virtual void OnSize(UINT nType, int cx, int cy) override;
    // 实现
public:
    virtual ~PanelView() { }

    virtual void RenderWithOpenGL();

    virtual VisualObject * GetObjectByGLName(VisualObject::GLNameType name);

};

