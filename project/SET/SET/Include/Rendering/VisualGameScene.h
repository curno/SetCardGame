#pragma once

#include "VisualScene.h"
#include "VisualCard.h"
#include "../Model/Game.h"

class VisualGameScene : public VisualScene
{
    static const int RowCount = 3;
    static const int ColumnCount = 7;
private:
    ref<Game> Game_;
    ref<VisualCard> Cards_[RowCount][ColumnCount]; // 3 * 7 = 21 visual card slots in the game scene, which can be empty.
public:
    VisualGameScene(ref<Game> game) : Game_(game) { }

protected:
    virtual void RenderContent() override;
    virtual void OnResize(const CSize &size) override;
private:
    void InitializeGameScene();
};