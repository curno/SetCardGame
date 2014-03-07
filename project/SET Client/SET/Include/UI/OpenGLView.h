#pragma once

#include "stdafx.h"
#include "../Rendering/VisualWidget.h"

class OpenGLView : public CView
{
    DECLARE_DYNCREATE(OpenGLView)
private:
    ref<VisualWidget> CurrentObject_;

public:
    OpenGLView();
public:
    virtual ~OpenGLView();
    virtual void OnDraw(CDC* pDC) override;
protected:
    DECLARE_MESSAGE_MAP()

    afx_msg virtual void OnDestroy();
    afx_msg virtual void OnSize(UINT nType, int cx, int cy);
    afx_msg virtual BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg virtual void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg virtual void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
    HGLRC GLRC_; // opengl rendering handle.

    // rendering.
    virtual void RenderWithOpenGL();
    virtual void InitOpenGL();

    // picking
    virtual void PickingWithOpenGL();
    virtual VisualWidget *GetWidgetByGLName(VisualObject::GLNameType name);
    virtual VisualWidget *GetWidgetByViewportPosition(CPoint position);
    VisualWidget *PickWidget(CPoint &point);

    // init opengl rc.
    BOOL InitGLRC();

    void MakeCurrent();
    void CancelCurrent();
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};