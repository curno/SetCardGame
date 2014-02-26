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
    // static functional methods.
    // decide if cards card1, card2, card3 make a Set.
    static bool IsSet(const CardRef card1, const CardRef card2, const CardRef card3)
    {
        if ((card1->Color != card2->Color || card1->Color != card3->Color) && // if three cards don't all have same color. And
            (card1->Color == card2->Color || card1->Color == card3->Color || card2->Color == card3->Color)) // if there are two cards which have same color
            return false;
        if ((card1->Symbol != card2->Symbol || card1->Symbol != card3->Symbol) && // same as color
            (card1->Symbol == card2->Symbol || card1->Symbol == card3->Symbol || card2->Symbol == card3->Symbol)) 
            return false;
        if ((card1->Shading != card2->Shading || card1->Shading != card3->Shading) && // same as color
            (card1->Shading == card2->Shading || card1->Shading == card3->Shading || card2->Shading == card3->Shading)) 
            return false;
        if ((card1->Number != card2->Number || card1->Number != card3->Number) && // same as color
            (card1->Number == card2->Number || card1->Number == card3->Number || card2->Number == card3->Number)) 
            return false;
        return true;
    }

    ~Card()
    {

    }

};
