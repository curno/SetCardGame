#pragma once

#include "stdafx.h"
#include "OpenGLView.h"
#include "../Rendering/VisualPanel.h"

// MainView ����
class MainView;
class PanelView : public OpenGLView
{
    DECLARE_DYNCREATE(PanelView)
    ref<VisualPanel> Panel_;
private:
    // ����
public:
    PanelView(VisualGameScene *scene = nullptr);

    // ��д
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;
    virtual void OnSize(UINT nType, int cx, int cy) override;
    // ʵ��
public:
    virtual ~PanelView() { }

    virtual void RenderWithOpenGL();

    virtual VisualObject * GetObjectByGLName(VisualObject::GLNameType name);

};

