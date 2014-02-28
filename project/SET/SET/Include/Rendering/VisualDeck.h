#pragma once

#include "VisualCard.h"
#include "Operation.h"
#include "TextureManager.h"
#include "../Animation/VisualObjectAnimations.h"
class VisualDeck : public VisualObject
{
    static const GLfloat NormalEmission;
    static const GLfloat HighlightEmission;
    ref<Operation> Operation_;
    ref<Animation> HoverAnimation_;
    ref<Animation> BlinkAnimation_;
public:
    VisualDeck() { Material_ = Material::GetMaterial("default"); SetNormalEmission(Material_); }
    ref<::Operation> readwrite(Operation);
    ref<::Operation> GetOperation() { return Operation_; }
    void SetOperation(ref<::Operation> operation) { Operation_ = operation; }
    virtual void OnMouseButtonDown() override
    {
        if (Operation_ != nullptr)
            (*Operation_)();
    }

protected:
    virtual void RenderContent() override
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        const double factor = VisualCard::DepthPerWidthRatio / (2 * VisualCard::DepthPerWidthRatio + 1);

        const double length = Size.Depth;
        const double mfactor = 1 - factor;

        Point p0(-Size.Width / 2, -Size.Height / 2, -Size.Depth / 2);
        Point p1 = -p0;

        glDisable(GL_BLEND);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_CARD_COVERED));
        // front
        glBegin(GL_QUADS);
        glNormal3d(0.0, 0.0, 1.0);
        glTexCoord2d(0.0, 0.0);
        glVertex3d(p0.X, p0.Y, p1.Z);
        glTexCoord2d(1.0, 0.0);
        glVertex3d(p1.X, p0.Y, p1.Z);
        glTexCoord2d(1.0, 1.0);
        glVertex3d(p1.X, p1.Y, p1.Z);
        glTexCoord2d(0.0, 1.0);
        glVertex3d(p0.X, p1.Y, p1.Z);
        glEnd();
        #pragma region Draw other five faces.
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_CARD_BACKGROUND));
        glBegin(GL_QUADS);
        // top
        glNormal3d(0.0, 1.0, 0.0);
        glTexCoord2d(mfactor, 1.0);
        glVertex3d(p1.X, p1.Y, p1.Z);
        glTexCoord2d(mfactor, mfactor);
        glVertex3d(p1.X, p1.Y, -p1.Z);
        glTexCoord2d(factor, mfactor);
        glVertex3d(p0.X, p1.Y, -p1.Z);
        glTexCoord2d(factor, 1.0);
        glVertex3d(p0.X, p1.Y, p1.Z);

        // bottom
        glNormal3d(0.0, -1.0, 0.0);
        glTexCoord2d(mfactor, 0.0);
        glVertex3d(p1.X, p0.Y, p1.Z);
        glTexCoord2d(factor, 0.0);
        glVertex3d(p0.X, p0.Y, p1.Z);
        glTexCoord2d(factor, factor);
        glVertex3d(p0.X, p0.Y, -p1.Z);
        glTexCoord2d(mfactor, factor);
        glVertex3d(p1.X, p0.Y, -p1.Z);

        // left
        glNormal3d(-1.0, 0.0, 0.0);
        glTexCoord2d(0, factor);
        glVertex3d(p0.X, p0.Y, p1.Z);
        glTexCoord2d(0, mfactor);
        glVertex3d(p0.X, p1.Y, p1.Z);
        glTexCoord2d(factor, mfactor);
        glVertex3d(p0.X, p1.Y, -p1.Z);
        glTexCoord2d(factor, factor);
        glVertex3d(p0.X, p0.Y, -p1.Z);

        // right
        glNormal3d(1.0, 0.0, 0.0);
        glTexCoord2d(1, factor);
        glVertex3d(p1.X, p0.Y, p1.Z);
        glTexCoord2d(mfactor, factor);
        glVertex3d(p1.X, p0.Y, -p1.Z);
        glTexCoord2d(mfactor, mfactor);
        glVertex3d(p1.X, p1.Y, -p1.Z);
        glTexCoord2d(1.0, mfactor);
        glVertex3d(p1.X, p1.Y, p1.Z);
        glEnd();


        // back
        glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_CARD_COVERED));
        glBegin(GL_QUADS);
        glNormal3d(0.0, 0.0, -1.0);
        glTexCoord2d(0.0, 0.0);
        glVertex3d(p0.X, p0.Y, -p1.Z);
        glTexCoord2d(1.0, 0.0);
        glVertex3d(p0.X, p1.Y, -p1.Z);
        glTexCoord2d(1.0, 1.0);
        glVertex3d(p1.X, p1.Y, -p1.Z);
        glTexCoord2d(0.0, 1.0);
        glVertex3d(p1.X, p0.Y, -p1.Z);
        glEnd();
        #pragma endregion

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        GLdouble mv[16];
        GLdouble pj[16];
        GLint vp[4];
        glGetDoublev(GL_MODELVIEW_MATRIX, mv);
        glGetDoublev(GL_PROJECTION_MATRIX, pj);
        glGetIntegerv(GL_VIEWPORT, vp);
        double x, y, z;
        gluProject(0.0, 0.0, 600, mv, pj, vp, &x, &y, &z);
    }

    virtual void OnMouseEnter() override
    {
        static const int HoverAnimationDuration = 500;
        static const double RotateTheta = -0.5;
        BlinkAnimation_ = nullptr;
        Highlight(Material_);

        Transformation transformation;
        transformation.RotateByCenter(-Size.Width / 2, Size.Height / 2, 0.0, 0.0, 0.0, 1.0, RotateTheta);
        HoverAnimation_ = MakeGenericAnimation(HoverAnimationDuration, ::Transform(GetTransformation(), transformation));
        HoverAnimation_->Start();
    }

    void Highlight(Material &m)
    {
        GLfloat emission[] = { HighlightEmission, HighlightEmission, HighlightEmission, 1.0f };
        m.SetData(Material::Parameter::Emission, emission);
    }

    virtual void OnMouseLeave() override
    {
        static const int HoverAnimationDuration = 500;
        BlinkAnimation_ = nullptr;
        SetNormalEmission(Material_);
        
        HoverAnimation_ = MakeGenericAnimation(HoverAnimationDuration, ::Transform(GetTransformation(), Transformation()));
        HoverAnimation_->Start();
    }

    void SetNormalEmission(Material &m)
    {
        GLfloat emission[] = { NormalEmission, NormalEmission, NormalEmission, 1.0f };
        m.SetData(Material::Parameter::Emission, emission);
    }
public:
    void Blink()
    {
        static const int duration = 1000;
        Material material = Material_;
        SetNormalEmission(material);
        Material material_highlight = Material_;
        Highlight(material_highlight);
        BlinkAnimation_ = MakeGenericAnimation(1000, ::Blink(Material_, material, material_highlight));
        BlinkAnimation_->Start();
    }
};