#pragma once

#include "../stdafx.h"
#include "VisualObject.h"

// VisualScene is a VisualObject which contains multiple VisualObjects as children.
// Use Composite design pattern here.
class VisualScene : public VisualObject
{
    ::std::vector<ref<VisualObject>> Children_;
public:
    virtual void RenderContent() override
    {
        // render each child
        for each (ref<VisualObject> child in Children_)
        {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslated(child->Position.x, child->Position.y, 0.0);
            child->Render();
            glPopMatrix();
        }
    }

    void AddChild(ref<VisualObject> child)
    {
        if (::std::find(Children_.begin(), Children_.end(), child) != Children_.end())
            return;
        Children_.push_back(child);
    }
};