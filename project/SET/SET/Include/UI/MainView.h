
// MainView.h : MainView 类的接口
//


#pragma once

#include "../Rendering/VisualGameScene.h"
#include "../Animation/GenericAnimation.h"
#include "../Animation/VisualObjectAnimations.h"
#include "../Animation/LoopAnimation.h"

// MainView 窗口

class MainView : public CView
{
    DECLARE_DYNCREATE(MainView)
private:
    ref<Game> Game_;
    ref<VisualGameScene> GameScene_;

    ref<VisualObject> CurrentObject_;
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
    virtual void OnDraw(CDC* pDC) override;
	// 生成的消息映射函数
protected:

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
    void InitOpenGL();
    BOOL InitGLRC(HDC hdc);
    VisualObject *PickObject(CPoint &point, int w, int h);
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    //static void CALLBACK EXPORT TimerProc(
    //    HWND hWnd,      // handle of CWnd that called SetTimer
    //    UINT nMsg,      // WM_TIMER
    //    UINT_PTR nIDEvent,   // timer identification
    //    DWORD dwTime    // system time
    //    );

    //typedef LRESULT(CALLBACK*WindowProcType)(
    //    _In_  HWND hwnd,
    //    _In_  UINT uMsg,
    //    _In_  WPARAM wParam,
    //    _In_  LPARAM lParam
    //    );

    //static WindowProcType proc;

    //static LRESULT CALLBACK WindowProcNew(
    //    _In_  HWND hwnd,
    //    _In_  UINT uMsg,
    //    _In_  WPARAM wParam,
    //    _In_  LPARAM lParam
    //    );
    //static MainView *view;
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

