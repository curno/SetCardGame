#pragma once

#include "stdafx.h"

// this class used to render text in the scene
// this is a singleton.
class TextRenderer
{
private:
    TextRenderer() { Init(); }
    TextRenderer(const TextRenderer &);
    TextRenderer &operator= (const TextRenderer &);

public:
    static TextRenderer &Instance()
    {
        static TextRenderer Instance_;
        return Instance_;
    }

    void DrawTextMy(const ::std::string &text, Point position)
    {
        CSize size = GetTextSize(text);
        glRasterPos3d(position.X - size.cx / 2.0, position.Y - size.cy / 2.0, position.Z);
        for each (char c in text)
            glCallList(CallListFirstID_ + static_cast<int>(c));
    }

    CSize GetTextSize(const ::std::string &text)
    {
        int width = 0;
        for each (char c in text)
            width += CharWidths_[static_cast<int>(c)];
        return CSize(width, CharHeight_);
    }
private:
    void Init()
    {
        CharHeight_ = 48;
        CallListFirstID_ = glGenLists(Count);
        /*HFONT hFont = CreateFontA(CharHeight_, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Comic Sans MS");
        HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
        DeleteObject(hOldFont);*/

        wglUseFontBitmaps(wglGetCurrentDC(), 0, Count, CallListFirstID_);
        GetCharWidth32(wglGetCurrentDC(), 0, Count - 1, CharWidths_);
    }
    static const int Count = 128;
    GLuint CallListFirstID_;
    int CharHeight_;
    int CharWidths_[Count];
};