#include "Include/stdafx.h"

#include "Include/Rendering/VisualScene.h"


void VisualScene::RenderContent()
{
    glPushName(0); // add a name to the name stack, the new position is used by all children.
    // render each child
    glMatrixMode(GL_MODELVIEW);
    for each (ref<VisualObject> child in Children_)
    {
        glPushMatrix(); // save current matrix
        glTranslated(child->Position.x, child->Position.y, 0.0); // move child to its position
        child->Render(); // render child
        glPopMatrix(); // restore
    }
    glPopName();
}

VisualObject *VisualScene::GetObjectByGLName(GLNameType name)
{
    auto *retval = __super::GetObjectByGLName(name); 
    if (retval)
        return retval;
    // if not found, search in children
    for each (ref<VisualObject> object in Children_)
    {
        auto *found = object->GetObjectByGLName(name);
        if (found != nullptr)
            return found;
    }
    return nullptr;
}

void VisualScene::AddChild(ref<VisualObject> child)
{
    if (::std::find(Children_.begin(), Children_.end(), child) != Children_.end())
        return;
    Children_.push_back(child);
}
