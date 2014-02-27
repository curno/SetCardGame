#pragma once

#include "stdafx.h"
#include "../Rendering/VisualObject.h"

class OpenGLView : public CView
{
    DECLARE_DYNCREATE(OpenGLView)
private:
    ref<VisualObject> CurrentObject_;

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
    afx_msg virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
    HGLRC GLRC_; // opengl rendering handle.
    virtual void RenderWithOpenGL();
    virtual void InitOpenGL();
    virtual VisualObject *GetObjectByGLName(VisualObject::GLNameType name);
    BOOL InitGLRC(HDC hdc);
    void MakeCurrent();
    void CancelCurrent();
    VisualObject *PickObject(CPoint &point, int w, int h);

};