
// MainView.h : MainView ��Ľӿ�
//


#pragma once

#include "../Rendering/VisualGameScene.h"
#include "../Rendering/VisualPanel.h"
#include "../Animation/GenericAnimation.h"
#include "../Animation/VisualObjectAnimations.h"
#include "../Animation/LoopAnimation.h"
#include "OpenGLView.h"

// MainView ����

class GameView : public OpenGLView
{
    DECLARE_DYNCREATE(GameView)
private:
    ref<Game> Game_;
    ref<VisualGameScene> GameScene_;
    ref<VisualPanel> ControlPanel_;
// ����
public:
	GameView();

// ����
public:

// ����
public:

// ʵ��
public:
	virtual ~GameView();
    ref<VisualGameScene> GetGameScene() { return GameScene_; }
protected:
    virtual void OnSize(UINT nType, int cx, int cy) override;

private:
    virtual void RenderWithOpenGL() override;
    virtual void InitOpenGL() override;
    virtual VisualObject *GetObjectByGLName(VisualObject::GLNameType name) override;
public:
    virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

