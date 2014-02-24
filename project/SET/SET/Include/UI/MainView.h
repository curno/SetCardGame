
// MainView.h : MainView 类的接口
//


#pragma once

#include "../Rendering/VisualGameScene.h"

// MainView 窗口

class MainView : public CWnd
{
private:
    ref<Game> Game_;
    ref<VisualGameScene> GameScene_;
// 构造
public:
	MainView();

// 特性
public:

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~MainView();

	// 生成的消息映射函数
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

