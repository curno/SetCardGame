#pragma once

#include "VisualObject.h"
#include "../Model/Card.h"
#include "Res/resource.h"

class VisualCard : public VisualObject
{
    static const int DefaultWidth = 100;
    static const int DefaultHeight = 200;
    CardRef Card_;

    BITMAP BITMAP_;
    GLuint TexName_;
    GLubyte image_2d[2][2][3];

    bool moved;
public:
    VisualCard(const CardRef card) : Card_(card), TexName_(0), moved(false)
    { 
        Size = CSize(DefaultWidth, DefaultHeight);

        CBitmap bitmap;
        bitmap.LoadBitmap(IDB_TEST);
        bitmap.GetBitmap(&BITMAP_);
        BITMAP_.bmBits = new byte[BITMAP_.bmWidthBytes * BITMAP_.bmHeight];
        bitmap.GetBitmapBits(BITMAP_.bmWidthBytes * BITMAP_.bmHeight, BITMAP_.bmBits);

    }

    void GenTexture()
    {
        glGenTextures(1, &TexName_);
        glBindTexture(GL_TEXTURE_2D, TexName_);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, BITMAP_.bmWidth, BITMAP_.bmHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, BITMAP_.bmBits);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    ~VisualCard() { delete[] BITMAP_.bmBits; }

    virtual void OnMouseMove() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;
protected:
    virtual void RenderContent() override;
    virtual void PrepareRendering() override;
    void SetCard(const CardRef card) { Card_ = card; }
};
