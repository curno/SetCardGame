#pragma once

#include "../stdafx.h"
#include "../Keyword.h"
// This is the Card class, represent a logic card in the table with its properties.
class Deck;
class Card;
typedef ref<Card> CardRef;
class Card
{
    // property types.
public:
    enum class ColorType
    {
        Red = 0,
        Green,
        Purple,
        // Count goes last.
        Count
    };
    enum class SymbolType
    {
        Squiggle = 0,
        Diamond,
        Oval,
        // Count goes last.
        Count
    };
    enum class ShadingType
    {
        Solid = 0,
        Striped,
        Outlined,
        // Count goes last.
        Count
    };
    enum class NumberType
    {
        One = 0,
        Two,
        Three,
        // Count goes last.
        Count
    };

    // properties
private:
    ColorType Color_;
    SymbolType Symbol_;
    ShadingType Shading_;
    NumberType Number_;
public:
    ColorType readonly(Color);
    ColorType GetColor() const { return Color_; }
    SymbolType readonly(Symbol);
    SymbolType GetSymbol() const { return Symbol_; }
    ShadingType readonly(Shading);
    ShadingType GetShading() const { return Shading_; }
    NumberType readonly(Number);
    NumberType GetNumber() const { return Number_; }
    int GetIntegerNumber() const { return static_cast<int>(Number) + 1; }
public:
    // operators
    bool operator== (const Card &that)
    {
        return Color == that.Color &&
            Symbol == that.Symbol &&
            Shading == that.Shading &&
            Number == that.Number;
    }

    bool operator!= (const Card &that)
    {
        return !operator== (that);
    }
private:
    friend class Deck;
    // constructor private, card is made by deck.
    Card() { }
    Card(const Card& other); // no copy constructor
    Card &operator= (const Card &other); // no assign operator
    void SetProperty(ColorType color, SymbolType symbol, ShadingType shading, NumberType number) 
    {
        Color_ = color;
        Symbol_ = symbol;
        Shading_ = shading;
        Number_ = number;
    }

public:
    static const int CardCountInASet = 3;
    // static functional methods.
    // decide if cards cards[0], cards[1], cards[2] make a Set.
    static bool IsSet(const ::std::vector<CardRef> &cards)
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

    // check cards in cards_to_check whether there is a SET in it.
    // if there is, return them by hint_cards and return true.
    // otherwise return false.
    static bool Hint(const ::std::unordered_set<CardRef> &cards_to_check, ::std::vector<CardRef> &hint_cards)
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
    ~Card()
    {

    }

};
