#pragma once

#include "../Keyword.h"
// This is the Card class, represent a logic card in the table with its properties.
class Deck;
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

private:
    friend class Deck;
    // constructor private, card is made by deck.
    Card() { }

    void SetProperty(ColorType color, SymbolType symbol, ShadingType shading, NumberType number)
    {
        Color_ = color;
        Symbol_ = symbol;

    }
};