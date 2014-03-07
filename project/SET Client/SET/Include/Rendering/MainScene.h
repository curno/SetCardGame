#pragma once

#include "VisualScene.h"
#include "../Model/Game.h"
#include "VisualGameScene.h"
#include "VisualPanel.h"

// this is the scene of the whole game UI
// it has two parts, the VisualGameScene which contains all the cards, and the VisualPanel that contains all control widgets.
// it also has the game object.
class MainScene : public VisualScene
{
    ref<Game> Game_; // Game object
    ref<VisualGameScene> GameScene_; // Game table
    ref<VisualPanel> ControlPanel_; // Game control panel.
public:
    MainScene();

protected:
    virtual void OnResize(const CSize &size) override;
    virtual void PrepareRendering();
};