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

public:

public:
	virtual ~MainView();
    ref<MainScene> GetScene() { return UI_; }
protected:
    virtual void OnSize(UINT nType, int cx, int cy) override;

private:
    virtual void RenderWithOpenGL() override;
    virtual VisualObject *GetObjectByGLName(VisualObject::GLNameType name) override;

};

