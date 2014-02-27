#include "Include/stdafx.h"

#include "Include/Model/Game.h"
#include "Include/Rendering/VisualGameScene.h"

Game::Game() : Scene_(nullptr)
{
    Deck_ = ref<Deck>(new Deck);
    Clear();
}

void Game::Start()
{
    Clear();
    State_ = State::Active;
    InitDeal();
    Watch_.Restart();
}

void Game::Pause()
{
    CTime current = CTime::GetCurrentTime();
    State_ = State::Paused;

    Watch_.Stop();
}

void Game::Resume()
{
    State_ = State::Active;

    Watch_.Start();
}
void Game::Stop()
{
    Pause();
    State_ = State::Stopped;

    Watch_.Stop();
}

void Game::DealMore()
{
    Deal(CardCountPerDeal);
}

bool Game::MoreToDeal()
{
    return CardsInHand_.size() >= CardCountPerDeal && CardsOnDesk_.size() < MaxCardsOnDesk;
}

bool Game::CheckAndScore(const ::std::vector<CardRef> &cards)
{
    // if there card make a set.
    if (Card::IsSet(cards))
    {
        // remove them from desk
        for each (CardRef card in cards)
            CardsOnDesk_.erase(CardsOnDesk_.find(card));

        // Score!
        Score_ += ScorePerSet;
        return true;
    }
    else
        return false;
}



CTimeSpan Game::GetTimeElapsed() const
{
    return Watch_.ElapsedMilliseconds / 1000;
}

void Game::Clear()
{
    State_ = State::Initilized;
    Score_ = 0;
    CardsOnDesk_.clear();
    CardsInHand_.clear();
    Watch_.Reset();
    Deck_->Shuffle();
    Deck_->SetAllCardsTo(CardsInHand_);
}

void Game::Deal(int card_count)
{
    ::std::unordered_set<CardRef> new_cards;
    auto it = CardsInHand_.begin();
    for (int i = 0; i < card_count; ++i)
    {
        // add to desk
        CardsOnDesk_.insert(*it);

        new_cards.insert(*it);

        // remove from hand
        CardsInHand_.erase(it++);
    }

    if (Scene_ != nullptr)
        Scene_->DealCards(new_cards);
}

void Game::SetScene(VisualGameScenePtr scene)
{
    Scene_ = scene;
}

Game::VisualGameScenePtr Game::GetScene() const
{
    return Scene_;
}

void Game::InitDeal()
{
    Deal(InitCardsCount);
}

bool Game::Hint(::std::vector<CardRef> &cards)
{
    return Card::Hint(CardsOnDesk_, cards);
}


