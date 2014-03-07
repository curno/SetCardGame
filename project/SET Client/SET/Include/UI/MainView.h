#pragma once

#include "../Rendering/MainScene.h"
#include "OpenGLView.h"

class MainView : public OpenGLView
{
    DECLARE_DYNCREATE(MainView)
private:
    ref<MainScene> UI_;
public:
	MainView();
public:
	virtual ~MainView();
    ref<MainScene> GetScene() { return UI_; }
protected:
    virtual void OnSize(UINT nType, int cx, int cy) override;

    virtual void RenderWithOpenGL() override;
    virtual void PickingWithOpenGL() override;

    virtual VisualWidget *GetWidgetByGLName(VisualObject::GLNameType name) override;
    virtual VisualWidget *GetWidgetByViewportPosition(CPoint position) override;
};

