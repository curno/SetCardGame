#include "Include/stdafx.h"

#include "Include/Model/Deck.h"

Deck::Deck()
{
    // for every color, shading, symbol and number, create a card
    int card_idx = 0;
    for (int color_idx = 0; color_idx < static_cast<int>(Card::ColorType::Count); ++color_idx)
        for (int symbol_idx = 0; symbol_idx < static_cast<int>(Card::SymbolType::Count); ++symbol_idx)
            for (int shading_idx = 0; shading_idx < static_cast<int>(Card::ShadingType::Count); ++shading_idx)
                for (int number_idx = 0; number_idx < static_cast<int>(Card::NumberType::Count); ++number_idx);


}