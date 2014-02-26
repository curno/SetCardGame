#pragma once

#include "stdafx.h"

#include "../Model/Card.h"
// this class manage all opengl texture objects from bitmaps in the program.
// it is a singleton
class TextureManager
{
public:
    typedef GLuint TextureName;
    typedef int ResourceID;
private:
    struct Texture
    {
        TextureName Name;
        BITMAP Data;
    };

    ::std::unordered_map<ResourceID, Texture> TextureMap_; // the map from resource IDs to opengl texture objects

private:
    TextureManager() { } // private constructor.
    TextureManager(const TextureManager &); // suppress copy constructor
    TextureManager &operator= (const TextureManager &); // suppress assign operator
public:
    TextureName GetTexture(ResourceID id)
    {
        auto i = TextureMap_.find(id);
        if (i == TextureMap_.end())
        {
            Texture t = CreateTexture(id);
            TextureMap_.insert(::std::make_pair(id, t));
            return t.Name;
        }
        return i->second.Name;
    }

    ~TextureManager()
    {
        for each (auto pair in TextureMap_)
            delete[] pair.second.Data.bmBits;
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

        TextureName name;
        glGenTextures(1, &name);
        retval.Name = name;

        glBindTexture(GL_TEXTURE_2D, name);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, retval.Data.bmWidth, retval.Data.bmHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, retval.Data.bmBits);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        return retval;
    }

    
};