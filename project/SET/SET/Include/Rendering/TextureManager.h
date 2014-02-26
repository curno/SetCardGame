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

private:
    TextureManager() { } // private constructor.
    TextureManager(const TextureManager &); // suppress copy constructor
    TextureManager &operator= (const TextureManager &); // suppress assign operator
public:
    TextureName GetTexture(ResourceID id)
    {
        auto i = TextureMap_.find(id);
        if (i != TextureMap_.end()) // already created.
            return i->second.Name;
        Texture t = CreateTexture(id);
        TextureMap_.insert(::std::make_pair(id, t));
        return t.Name;
    }
    TextureName GetTexture(CardRef card) 
    { 
        assert(card != nullptr);
        CardParameter parameter = { card->Symbol, card->Shading, card->Color };
        auto index = CardTextureMap_.find(parameter);
        if (index != CardTextureMap_.end())
            return index->second.Name;
        Texture t = CreateTexture(parameter);
        CardTextureMap_.insert(::std::make_pair(parameter, t));
        return t.Name;

    }
    ~TextureManager()
    {
        for each (auto pair in TextureMap_)
            delete[] pair.second.Data.bmBits;
        for each (auto pair in CardTextureMap_)
            delete pair.second.Data.bmBits;
    }

    // singleton
    static TextureManager &Instance()
    {
        static TextureManager Instance_;
        return Instance_;
    }
private:
    Texture CreateTexture(ResourceID id)
    {
        Texture retval;
        CBitmap bitmap;
        bitmap.LoadBitmap(id);
        bitmap.GetBitmap(&retval.Data);
        retval.Data.bmBits = new byte[retval.Data.bmWidthBytes * retval.Data.bmHeight];

        bitmap.GetBitmapBits(retval.Data.bmWidthBytes * retval.Data.bmHeight, retval.Data.bmBits);
        //FillAlphaElement(retval);

        CreateNameAndTexture(retval, false);
        return retval;
    }

    Texture CreateTexture(const CardParameter &param)
    {
        static ColorElement TransparentKey[] = { 0, 0, 0, 0 };
        static ColorElement TransparentColor[] = { 255, 255, 255, 0 };
        for each (auto &i in CardTextureMap_)
        {
            // if there already exists a texture which has same symbol and shading
            // just change the color.
            if (i.first.Symbol == param.Symbol && i.first.Shading == param.Shading)
            {
                ColorElement color[4];
                GetColorData(param.Color, color);
                return CreateTexture(i.second, color, TransparentColor, TransparentColor);
            }
        }

        // no texture has same symbol and shading
        // create texture from file.
        int id = GetResourceID(param.Symbol, param.Shading);
        Texture t;
        CBitmap bitmap;
        bitmap.LoadBitmap(id);
        bitmap.GetBitmap(&t.Data);
        t.Data.bmBits = new byte[t.Data.bmWidthBytes * t.Data.bmHeight];
        bitmap.GetBitmapBits(t.Data.bmWidthBytes * t.Data.bmHeight, t.Data.bmBits);
        FillAlphaElement(t, TransparentKey);
        ColorElement color[4];
        GetColorData(param.Color, color);
        Texture retval = CreateTexture(t, color, TransparentKey, TransparentColor);
        delete[] t.Data.bmBits;
        return retval;
    }

    Texture CreateTexture(const Texture &t, ColorElement *c, ColorElement *transparent_key, ColorElement *transparent_color)
    {
        Texture retval;

        memcpy(&retval.Data, &t.Data, sizeof(retval.Data));
        retval.Data.bmBits = new byte[t.Data.bmWidthBytes * t.Data.bmHeight];

        ColorElement *source = static_cast<ColorElement *>(t.Data.bmBits);
        ColorElement *dest = static_cast<ColorElement *>(retval.Data.bmBits);

        for (int h = 0; h < t.Data.bmHeight; ++h)
        {
            ColorElement *source_row = source;
            ColorElement *dest_row = dest;
            for (int w = 0; w < t.Data.bmWidth; ++w)
            {
                if (!SameColor(source_row, transparent_key))
                {
                    /*double factor = 0.0;
                    factor = max(factor, source_row[0] / 255.0);
                    factor = max(factor, source_row[1] / 255.0);
                    factor = max(factor, source_row[2] / 255.0);
                    dest_row[0] = min(255, max(0, static_cast<ColorElement>(c[0] * factor)));
                    dest_row[1] = min(255, max(0, static_cast<ColorElement>(c[1] * factor)));
                    dest_row[2] = min(255, max(0, static_cast<ColorElement>(c[2] * factor)));
                    dest_row[3] = c[3];*/
                    CopyColor(dest_row, c);
                }
                else
                    CopyColor(dest_row, transparent_color);
                dest_row += 4;
                source_row += 4;
            }
            source += t.Data.bmWidthBytes;
            dest += t.Data.bmWidthBytes;
        }

        CreateNameAndTexture(retval, true);
        return retval;
    }

    void FillAlphaElement(Texture &retval, ColorElement *transparent_key)
    {
        ColorElement *data = static_cast<ColorElement *>(retval.Data.bmBits);
        for (int h = 0; h < retval.Data.bmHeight; ++h)
        {
            ColorElement *data_row = data;
            for (int w = 0; w < retval.Data.bmWidth; ++w)
            {
                if (SameColor(data_row, transparent_key))
                    data_row[3] = 0; // set alpha to 255;
                else
                    data_row[3] = 255;
                data_row += 4;
            }
            data += retval.Data.bmWidthBytes;
        }
    }

    void CreateNameAndTexture(Texture &texture, bool alpha)
    {
        TextureName name;
        glGenTextures(1, &name);
        texture.Name = name;
        glBindTexture(GL_TEXTURE_2D, texture.Name);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, texture.Data.bmWidth, texture.Data.bmHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texture.Data.bmBits);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    bool SameColor(const ColorElement *c1, const ColorElement *c2)
    {
        return *(const int *)(c1) == *(const int *)(c2); // C Style cast, dangerous!!
    }

    void CopyColor(ColorElement *dest, const ColorElement *source)
    {
        *(int *)(dest) = *(const int *)(source); // C Style cast, dangerous!!
    }

    // 
    void GetColorData(Card::ColorType color, ColorElement *data)
    {
        static const ColorElement Red[] = { 255, 0, 0, 255 };
        static const ColorElement Green[] = { 0, 255, 0, 255 };
        static const ColorElement Purple[] = { 255, 0, 255, 255 };
        const ColorElement *source = nullptr;
        switch (color)
        {
        case Card::ColorType::Red:
            source = Red;
            break;
        case Card::ColorType::Green:
            source = Green;
            break;
        case Card::ColorType::Purple:
            source = Purple;
            break;
        default:
            break;
        }
        data[0] = source[0];
        data[1] = source[1];
        data[2] = source[2];
        data[3] = source[3];
    }

    int GetResourceID(Card::SymbolType symbol, Card::ShadingType shading)
    {
        switch (symbol)
        {
        case Card::SymbolType::Squiggle:
            switch (shading)
            {
            case Card::ShadingType::Solid:
                return IDB_SQUIGGLE_SOLID;
            case Card::ShadingType::Striped:
                return IDB_SQUIGGLE_STRIPED;
            case Card::ShadingType::Outlined:
                return IDB_SQUIGGLE_OUTLINED;
            }
        case Card::SymbolType::Diamond:
            switch (shading)
            {
            case Card::ShadingType::Solid:
                return IDB_DIAMOND_SOLID;
            case Card::ShadingType::Striped:
                return IDB_DIAMOND_STRIPED;
            case Card::ShadingType::Outlined:
                return IDB_DIAMOND_OUTLINED;
            }
        case Card::SymbolType::Oval:
            switch (shading)
            {
            case Card::ShadingType::Solid:
                return IDB_OVAL_SOLID;
            case Card::ShadingType::Striped:
                return IDB_OVAL_STRIPED;
            case Card::ShadingType::Outlined:
                return IDB_OVAL_OUTLINED;
            }
        }
        return IDB_TEST;
    }
};