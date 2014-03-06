#pragma once

#include "VisualScene.h"
#include "VisualDeck.h"
#include "VisualButton.h"
#include "VisualGameScene.h"
#include "../../Res/resource.h"
#include "../Web/HttpServer.h"
#include "SET.h"
#include "../UI/ScoreDialog.h"

class VisualPanel : public VisualScene
{
    ref<VisualButton> ButtonNewGame_;
    ref<VisualButton> ButtonCommit_;
    ref<VisualButton> ButtonHint_;
    ref<VisualDeck> Deck_;
    VisualGameScene *GameScene_; // the scene this panel will control.
protected:
    virtual void RenderContent() override;
    virtual void OnResize(const CSize &size);

public:
    VisualPanel(VisualGameScene *scene);

    void DeckClicked();
    void ButtonNewGameClicked();
    void ButtonHintClicked();
    void ButtonSubmitClicked();
    void StopAndSubmit();
private:
    static const int ButtonWidth = 80;
    static const int Margin = 15;
    static const int DigitWidth = 20;
    static const int DigitHeight = 40;
    void RenderDigit(int number, const Point &p, const Dimension& size);

    // render number, p is the position of the last number.
    void RenderNumber(int number, const Point &p, int least_width);
};