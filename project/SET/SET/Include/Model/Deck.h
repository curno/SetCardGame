#pragma once

#include "Card.h"

typedef int CardRef;
// this class represent a logic deck of 81 cards.
class Deck
{
private:
    static const int Total = 
        static_cast<int>(Card::ColorType::Count) * 
        static_cast<int>(Card::SymbolType::Count) *
        static_cast<int>(Card::ShadingType::Count) *
        static_cast<int>(Card::NumberType::Count); // = 81
    Card *Cards_[Total]; // all cards.

    void InitCards();
public:

    Deck() { InitCards(); }
    ~Deck();
    const Card &GetCard(CardRef index) const { return *Cards_[index]; }
    int GetCardsCount() const { return Total; }
    void Shuffle() { ::std::random_shuffle(Cards_, Cards_ + Total); }
};

