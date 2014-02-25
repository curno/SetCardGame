#pragma once

#include "../stdafx.h"
#include "Deck.h"
#include "Stopwatch.h"

class VisualGameScene;
class Game
{
    typedef int ScoreType;
    static const ScoreType ScorePerSet = 3; // Each set give 3 points.
    static const int CardCountPerDeal = 3; // Deal more 3 cards every time.
    static const int MaxCardsOnDesk = 21; // At one time, at most 21 cards on desk.
private:
    ref<Deck> Deck_; // cards
    ::std::unordered_set<CardRef> CardsInHand_; // card refs in hand, which can be dealed.
    ::std::unordered_set<CardRef> CardsInDesk_; // card refs currently stay on desk, which can be selected.

    ScoreType Score_; // game score

    mutable Stopwatch Watch_; // the stop watch to get the elapsed time.
    enum class State
    {
        Initilized, Active, Paused, Stopped
    } State_; // game state

    // One scene that observe the change of the game. OBSERVER design pattern.
    // The game does NOT own the scene, so it is a weak ref.
    VisualGameScene *Scene_; 
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

    typedef VisualGameScene *VisualGameScenePtr;
    VisualGameScenePtr readwrite(Scene);
    VisualGameScenePtr GetScene() const;
    void SetScene(VisualGameScenePtr scene);
private:
    void Clear();
    void Deal(int card_count);
};


