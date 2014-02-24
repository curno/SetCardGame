
// MainView.h : MainView ��Ľӿ�
//


#pragma once

#include "../Rendering/VisualGameScene.h"

// MainView ����

class MainView : public CWnd
{
private:
    ref<Game> Game_;
    ref<VisualGameScene> GameScene_;
// ����
public:
	MainView();

// ����
public:

// ����
public:

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~MainView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();

    void InitOpenGL();


    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
    HGLRC GLRC_; // opengl rendering handle.
    void RenderWithOpenGL();
public:
};

