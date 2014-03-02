#pragma once

#include "../stdafx.h"
#include "Deck.h"
#include "../Utils/Stopwatch.h"

class VisualGameScene;
class Game
{
public:
    typedef int ScoreType;
    enum class State
    {
        Initilized, Active, Paused, Stopped
    };

    static const ScoreType ScorePerSet = 3; // Each set give 3 points.
    static const int CardCountPerDeal = Card::CardCountInASet; // Deal more 3 cards every time.
    static const int MaxCardsOnDesk = 21; // At one time, at most 21 cards on desk.
    static const int InitCardsCount = 12;
private:
    ref<Deck> Deck_; // cards
    ::std::unordered_set<CardRef> CardsInHand_; // card refs in hand, which can be dealed.
    ::std::unordered_set<CardRef> CardsOnDesk_; // card refs currently stay on desk, which can be selected.

    ScoreType Score_; // game score

    mutable Stopwatch Watch_; // the stop watch to get the elapsed time.
    
    State State_; // game state
   

public:
    Game();
    void Start();
    void Pause();
    void Resume();
    void Stop();
    void DealMore(::std::unordered_set<CardRef> &new_cards);
    bool MoreToDeal();
    bool CheckAndScore(const ::std::vector<CardRef> &cards);
    bool Hint(::std::vector<CardRef> &cards);
    CTimeSpan readonly(TimeElapsed);
    CTimeSpan GetTimeElapsed() const;

    State readonly(GameState);
    State GetGameState() const { return State_; }

    ::std::unordered_set<CardRef> &GetCardsOnDesk() { return CardsOnDesk_; }
    ScoreType readonly(Score);
    ScoreType GetScore() const { return Score_; }

    int GetCardsInHandCount() { return static_cast<int>(CardsInHand_.size()); }
    int GetCardsOnDeskCount() { return static_cast<int>(CardsOnDesk_.size()); }
    int GetCardsTotalCount() { return Deck::Total; }
private:
    void Clear();
    void Deal(int card_count, ::std::unordered_set<CardRef> &new_cards);
    void InitDeal(::std::unordered_set<CardRef> &new_cards);
};


