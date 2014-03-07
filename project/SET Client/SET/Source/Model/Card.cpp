#include "Include/stdafx.h"

#include "Include/Model/Card.h"

bool Card::Hint(const ::std::unordered_set<CardRef> &cards_to_check, ::std::vector<CardRef> &hint_cards)
{
    ::std::vector<CardRef> current(CardCountInASet);
    for (auto i = cards_to_check.begin(); i != cards_to_check.end(); ++i)
    {
        current[0] = *i;
        auto j = i;
        ++j;
        for (; j != cards_to_check.end(); ++j)
        {
            current[1] = *j;
            auto k = j;
            ++k;
            for (; k != cards_to_check.end(); ++k)
            {
                current[2] = *k;
                // for every three different cards, if they are a SET, return.
                if (Card::IsSet(current))
                {
                    copy(current.begin(), current.end(), back_inserter(hint_cards));
                    return true;
                }
            }
        }
    }
    return false;
}

bool Card::IsSet(const ::std::vector<CardRef> &cards)
{
    if (cards.size() != CardCountInASet)
        return false;

    if ((cards[0]->Color != cards[1]->Color || cards[0]->Color != cards[2]->Color) && // if three cards don't all have same color. And
        (cards[0]->Color == cards[1]->Color || cards[0]->Color == cards[2]->Color || cards[1]->Color == cards[2]->Color)) // if there are two cards which have same color
        return false;
    if ((cards[0]->Symbol != cards[1]->Symbol || cards[0]->Symbol != cards[2]->Symbol) && // same as color
        (cards[0]->Symbol == cards[1]->Symbol || cards[0]->Symbol == cards[2]->Symbol || cards[1]->Symbol == cards[2]->Symbol))
        return false;
    if ((cards[0]->Shading != cards[1]->Shading || cards[0]->Shading != cards[2]->Shading) && // same as color
        (cards[0]->Shading == cards[1]->Shading || cards[0]->Shading == cards[2]->Shading || cards[1]->Shading == cards[2]->Shading))
        return false;
    if ((cards[0]->Number != cards[1]->Number || cards[0]->Number != cards[2]->Number) && // same as color
        (cards[0]->Number == cards[1]->Number || cards[0]->Number == cards[2]->Number || cards[1]->Number == cards[2]->Number))
        return false;
    return true;
}

void Card::SetProperty(ColorType color, SymbolType symbol, ShadingType shading, NumberType number)
{
    Color_ = color;
    Symbol_ = symbol;
    Shading_ = shading;
    Number_ = number;
}
