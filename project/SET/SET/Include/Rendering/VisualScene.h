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
        glPushName(0); // add a name to the name stack, the new position is used by all children.
        // render each child
        glMatrixMode(GL_MODELVIEW);
        for each (ref<VisualObject> child in Children_)
        {
            glPushMatrix();
            glTranslated(child->Position.x, child->Position.y, 0.0);
            child->Render();
            glPopMatrix();
        }
        glPopName(); 
    }

    void AddChild(ref<VisualObject> child)
    {
        if (::std::find(Children_.begin(), Children_.end(), child) != Children_.end())
            return;
        Children_.push_back(child);
    }

    virtual VisualObject *GetObjectByGLName(GLNameType name) override
    {
        auto *retval = __super::GetObjectByGLName(name);
        if (retval)
            return retval;
        for each (ref<VisualObject> object in Children_)
        {
            auto *found = object->GetObjectByGLName(name);
            if (found != nullptr)
                return found;
        }
        return nullptr;
    }
};