#pragma once

#include "stdafx.h"

#include "../Model/Card.h"
#include "../Res/resource.h"
// this class manage all opengl texture objects from bitmaps in the program.
// it is a singleton
class TextureManager
{
public:
    typedef GLuint TextureName;
    typedef int ResourceID;
    typedef unsigned char ColorElement;

    static const int Slash = 10; // Slash pixel map ID
    static const int Colon = 11; // Colon pixel map ID
private:
    struct Texture
    {
        TextureName Name;
        BITMAP Data;
    };

    struct CardParameter
    {
        ::Card::SymbolType Symbol;
        ::Card::ShadingType Shading;
        ::Card::ColorType Color;

        bool operator== (const CardParameter &other) const
        {
            return Symbol == other.Symbol && Shading == other.Shading && Color == other.Color;
        }

        bool operator!= (const CardParameter &other) const
        {
            return !operator==(other);
        }

        struct Hasher
        {
            size_t operator() (const CardParameter &p)
            {
                return (size_t)(p.Symbol) | 
                    ((size_t)(p.Shading) << 8) | 
                    ((size_t)(p.Color) << 16);
            }
            
        };
    };
    ::std::unordered_map<ResourceID, Texture> TextureMap_; // the map from resource IDs to opengl texture objects
    ::std::unordered_map<CardParameter, Texture, CardParameter::Hasher> CardTextureMap_; // the map from a card parameter to a texture object.
    ::std::unordered_map<int, Texture> NumberTextureMap_; // the map from a number(0~9) to a texture object.

private:
    TextureManager() { } // private constructor.
    TextureManager(const TextureManager &); // suppress copy constructor
    TextureManager &operator= (const TextureManager &); // suppress assign operator
public:
    // Get texture for specified resource ID
    TextureName GetTexture(ResourceID id);

    // Get texture for a card
    // The texture manager will check if there already has a suitable texture,
    // If no, create a new texture by varying symbol or shading or color.
    TextureName GetTexture(CardRef card);

    // Get texture for a single digit or slash or colon.
    TextureName GetDigitTexture(int number);

    // Release all textures.
    ~TextureManager();

    // singleton
    static TextureManager &Instance();
private:

    // Create texture for a digit
    Texture CreateTextureForDigit(int number);

    // Create texture from a resource bmp.
    Texture CreateTexture(ResourceID id);

    // Create texture object for specified card parameters
    Texture CreateTexture(const CardParameter &param);

    // Create texture object using existing texture t, 
    // This method will treat all the transparent_key pixels in texture t as transparent and set them to transparent_color,
    // and treat all pixels that are not transparent to be c.
    Texture CreateTexture(const Texture &t, ColorElement *c, ColorElement *transparent_key, ColorElement *transparent_color);

    // Set the alpha value of all transparent_key pixels in retval to be 0 and others 255.
    void FillAlphaElement(Texture &retval, ColorElement *transparent_key);

    // Assign a opengl texture name for texture and create it, alpha indicates whether the texture will have a alpha channel.
    void AssignNameAndCreateTexture(Texture &texture, bool alpha);

    // utils.
    bool SameColor(const ColorElement *c1, const ColorElement *c2);
    void CopyColor(ColorElement *dest, const ColorElement *source);

    // Get the corresponding resouce ID for specified symbol and shading.
    int GetResourceID(Card::SymbolType symbol, Card::ShadingType shading);
    // Get color for specified Card ColorType.
    void GetColorData(Card::ColorType color, ColorElement *data);
};
