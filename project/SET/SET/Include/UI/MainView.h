
// MainView.h : MainView 类的接口
//


#pragma once

#include "../Rendering/MainScene.h"
#include "OpenGLView.h"

// MainView 窗口

class MainView : public OpenGLView
{
    DECLARE_DYNCREATE(MainView)
private:
    ref<MainScene> UI_;
// 构造
public:
	MainView();

// 特性
public:

// 操作
public:

// 实现
public:
	virtual ~MainView();
    ref<MainScene> GetScene() { return UI_; }
protected:
    virtual void OnSize(UINT nType, int cx, int cy) override;

private:
    virtual void RenderWithOpenGL() override;
    virtual VisualObject *GetObjectByGLName(VisualObject::GLNameType name) override;
};

