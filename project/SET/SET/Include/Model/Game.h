#pragma once

#include "../stdafx.h"
#include "Deck.h"
#include "Stopwatch.h"


class Game
{

    typedef int ScoreType;
    static const ScoreType ScorePerSet = 3; // Each set give 3 points.
    static const int CardCountPerDeal = 3; // Deal more 3 cards everytime.
private:
    ref<Deck> Deck_; // cards
    ::std::set<CardRef> CardsInHand_; // card refs in hand, which can be dealed.
    ::std::set<CardRef> CardsInDesk_; // card refs currently stay on desk, which can be selected.

    ScoreType Score_; // game score

    mutable Stopwatch Watch_; // the stop watch to get the elapsed time.
    enum class State
    {
        Initilized, Active, Paused, Stopped
    } State_; // game state
public:
    Game();
    void Start();
    void Pause();
    void Resume();
    void Stop();
    void DealMore();
    bool MoreToDeal();
    bool CheckAndScore(CardRef card1, CardRef card2, CardRef card3);

    CTimeSpan readonly(TimeElapsed);
    CTimeSpan GetTimeElapsed() const;

private:
    void Clear();
    void Deal(int card_count);
};


