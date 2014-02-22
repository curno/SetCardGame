#pragma once

#include "Card.h"

// this class represent a logic deck of 81 cards.
class Deck
{
private:
    static const int Total = 
        static_cast<int>(Card::ColorType::Count) * 
        static_cast<int>(Card::SymbolType::Count) *
        static_cast<int>(Card::ShadingType::Count) *
        static_cast<int>(Card::NumberType::Count); // = 81
    CardRef Cards_[Total]; // all cards.

    void InitCards();
public:

    Deck() { InitCards(); }
    ~Deck();
    const CardRef GetCard(int index) const { return Cards_[index]; }
    int GetCardsCount() const { return Total; }
    void Shuffle() { ::std::random_shuffle(Cards_, Cards_ + Total); }
};

