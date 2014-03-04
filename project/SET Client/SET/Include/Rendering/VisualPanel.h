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
    virtual void RenderContent() override
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_PANEL_BACKGROUND));
        double w = Size.Width;
        double h = Size.Height;

        // first render panel.
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0);
        glVertex2d(-w / 2.0, -h / 2.0);
        glTexCoord2d(1.0, 0.0);
        glVertex2d(w / 2.0, -h / 2.0);
        glTexCoord2d(1.0, 1.0);
        glVertex2d(w / 2.0, h / 2.0);
        glTexCoord2d(0.0, 1.0);
        glVertex2d(-w / 2.0, h / 2.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        // digit size
        Dimension size(DigitWidth, DigitHeight, 10);

        // time
        CTimeSpan span = GameScene_->GetGame()->GetTimeElapsed();
        Point time_point(-size.Width, 0, size.Depth / 2);
        RenderNumber(span.GetMinutes(), time_point, 2);
        time_point.X = 0;
        RenderDigit(TextureManager::Colon, time_point, size);
        time_point.X = size.Width * 2;
        RenderNumber(span.GetSeconds(), time_point, 2);

        // if game started, render score.
        if (GameScene_->GetGame()->GameState != Game::State::Initilized)
        {
            Point score_point(-Size.Width / 2 + Margin * 2 + Deck_->Size.Width + 2 * size.Width, 0, 10);
            RenderNumber(GameScene_->GetGame()->Score, score_point, 2);
            score_point.X += size.Width;
            RenderDigit(TextureManager::Slash, score_point, size);
            score_point.X += 2 * size.Width;
            RenderNumber(GameScene_->GetGame()->GetCardsTotalCount(), score_point, 2);
        }
        
        __super::RenderContent();

    }

    virtual void PrepareRendering()
    {
        __super::PrepareRendering();
    }

    virtual void OnResize(const CSize &size)
    {
        double button_height = ButtonWidth * VisualButton::HeightPerWidthRatio;
        double deck_height = size.cy - 2 * Margin;
        double deck_width = deck_height * VisualCard::HeightPerWidthRatio;
        Deck_->Size = Dimension(static_cast<Coordinate>(deck_width),
            static_cast<Coordinate>(deck_height),
            static_cast<Coordinate>(VisualCard::DepthPerWidthRatio * deck_height));
        Deck_->Position = Point(Margin - Size.Width / 2 + Deck_->Size.Width / 2, 0, Deck_->Size.Depth / 2);

        Dimension button_size = Dimension(ButtonWidth,
            static_cast<Coordinate>(ButtonWidth * VisualButton::HeightPerWidthRatio),
            static_cast<Coordinate>(ButtonWidth * VisualButton::DepthPerWidthRatio)
            );
        ButtonNewGame_->Size = button_size;
        ButtonHint_->Size = button_size;
        ButtonCommit_->Size = button_size;

        double margin = (Deck_->Size.Height - 3 * button_height) / 2.0;
        ButtonNewGame_->Position = Point(static_cast<Coordinate>(size.cx / 2 - Margin * 3 - ButtonWidth * 2.5),
            0,
            button_size.Depth / 2);
        ButtonCommit_->Position = Point(static_cast<Coordinate>(size.cx / 2 - Margin - ButtonWidth * 0.5),
            0,
            button_size.Depth / 2);
        ButtonHint_->Position = Point(static_cast<Coordinate>(size.cx / 2 - Margin * 2 - ButtonWidth * 1.5),
            0,
            button_size.Depth / 2);
    }

public:
    VisualPanel(VisualGameScene *scene) : GameScene_(scene)
    {
        Material_ = Material::GetMaterial("default");
        ButtonNewGame_ = ::std::make_shared<VisualButton>(MakeGenericOperation([this](){ ButtonNewGameClicked(); }));
        ButtonNewGame_->SetTextureName(IDB_NEW_GAME);
        ButtonHint_ = ::std::make_shared<VisualButton>(MakeGenericOperation([this](){ ButtonHintClicked(); }));
        ButtonHint_->SetTextureName(IDB_HINT);
        ButtonHint_->Enabled = false;
        ButtonCommit_ = ::std::make_shared<VisualButton>(MakeGenericOperation([this](){ ButtonSubmitClicked(); }));
        ButtonCommit_->SetTextureName(IDB_COMMIT);
        ButtonCommit_->Enabled = false;
        Deck_ = ::std::make_shared<VisualDeck>();
        Deck_->Operation = MakeGenericOperation([this](){DeckClicked();});
        Deck_->Visible = false;
        AddChild(ButtonNewGame_);
        AddChild(ButtonHint_);
        AddChild(ButtonCommit_);
        AddChild(Deck_);
    }

    void DeckClicked()
    {
        GameScene_->Deal();
        if (GameScene_->GetGame()->GetCardsInHandCount() == 0)
            Deck_->Visible = false;
    }

    void ButtonNewGameClicked()
    {
        GameScene_->Start(); 
        Deck_->Enabled = true; 
        Deck_->Visible = true;
        ButtonHint_->Enabled = true;
        ButtonCommit_->Enabled = true;
    }

    void ButtonHintClicked()
    {
        if (!GameScene_->Hint())
        {
            if (GameScene_->GetGame()->MoreToDeal())
                Deck_->Blink();
        }
    }

    void ButtonSubmitClicked()
    {
        if (GameScene_->GetGame()->GameState == Game::State::Active)
            StopAndCommit();
        else if (GameScene_->GetGame()->GameState == Game::State::Initilized)
            MessageBox(theApp.m_pMainWnd->m_hWnd, TEXT("You have no score yet, please start a game first."), TEXT("Uh.."), NULL);
        else if (GameScene_->GetGame()->GameState == Game::State::Stopped)
            MessageBox(theApp.m_pMainWnd->m_hWnd, TEXT("No active game now, lease start a game first."), TEXT("Uh.."), NULL);
    }

    void StopAndCommit()
    {
        GameScene_->Stop();
        ButtonHint_->Enabled = false;
        Deck_->Enabled = false;
        ButtonCommit_->Enabled = false;
       
        HttpServer::DataCallBackFunction = ScoreDialog::AnalysisAndShowScoreDialog;
        HttpServer::Post(theApp.PlayerName, GameScene_->GetGame()->Score, GameScene_->GetGame()->TimeElapsed.GetTotalSeconds());
    }

private:
    static const int ButtonWidth = 80;
    static const int Margin = 15;
    static const int DigitWidth = 20;
    static const int DigitHeight = 40;
    void RenderDigit(int number, const Point &p, const Dimension& size)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetNumberTexture(number));
        glDisable(GL_LIGHTING);
        glTranslated(p.X, p.Y, p.Z);
        glColor4d(1.0, 0.0, 1.0, 1.0);
        Point p0(-size.Width / 2, -size.Height / 2, -size.Depth / 2);
        Point p1 = -p0;
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 1.0);
        glVertex3d(p0.X, p0.Y, p1.Z);
        glTexCoord2d(1.0, 1.0);
        glVertex3d(p1.X, p0.Y, p1.Z);
        glTexCoord2d(1.0, 0.0);
        glVertex3d(p1.X, p1.Y, p1.Z);
        glTexCoord2d(0.0, 0.0);
        glVertex3d(p0.X, p1.Y, p1.Z);
        glEnd();
        glTranslated(-p.X, -p.Y, -p.Z);
        glColor4d(1.0, 1.0, 1.0, 1.0);
        glDisable(GL_BLEND);

    }

    // render number, p is the position of the last number.
    void RenderNumber(int number, const Point &p, int least_width)
    {
        Point positon = p;
        int width = 0;
        Dimension digit_size(DigitWidth, DigitHeight, 10);
        while (number != 0)
        {
            int digit = number % 10;
            number /= 10;
            RenderDigit(digit, positon, digit_size);
            positon.X -= DigitWidth;
            width++;
        }

        for (int i = width; i < least_width; ++i)
        {
            RenderDigit(0, positon, digit_size);
            positon.X -= DigitWidth;
        }
    }
};