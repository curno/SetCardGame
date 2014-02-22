#pragma once

#include "Card.h"

// this class represent a deck of 81 cards.
class Deck
{
private:
    static const int Total = 
        static_cast<int>(Card::ColorType::Count) * 
        static_cast<int>(Card::SymbolType::Count) *
        static_cast<int>(Card::ShadingType::Count) *
        static_cast<int>(Card::NumberType::Count);
    Card Cards_[Total]; // all cards.
public:
    Deck();
    const Card &GetCard(int index) const { return Cards_[index]; }
    int GetCardsCount() const { return Total; }
};