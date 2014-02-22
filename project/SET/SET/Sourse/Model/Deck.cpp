#include "Include/stdafx.h"

#include "Include/Model/Deck.h"

void Deck::InitCards()
{
    // for every color, shading, symbol and number, create a card
    int card_idx = 0;
    for (int color_idx = 0; color_idx < static_cast<int>(Card::ColorType::Count); ++color_idx)
        for (int symbol_idx = 0; symbol_idx < static_cast<int>(Card::SymbolType::Count); ++symbol_idx)
            for (int shading_idx = 0; shading_idx < static_cast<int>(Card::ShadingType::Count); ++shading_idx)
                for (int number_idx = 0; number_idx < static_cast<int>(Card::NumberType::Count); ++number_idx)
                {
                    Cards_[card_idx] = new Card;
                    Cards_[card_idx]->SetProperty(Card::ColorType(color_idx),
                        Card::SymbolType(symbol_idx),
                        Card::ShadingType(shading_idx),
                        Card::NumberType(number_idx));
                    ++card_idx;
                }
}

Deck::~Deck()
{
    for (int i = 0; i < Total; ++i)
        delete Cards_[i];
}
