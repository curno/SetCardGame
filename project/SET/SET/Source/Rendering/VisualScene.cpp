#include "Include/stdafx.h"

#include "Include/Rendering/VisualScene.h"


void VisualScene::RenderContent()
{
    glPushName(0); // add a name to the name stack, the new position is used by all children.
    // render each child
    glMatrixMode(GL_MODELVIEW);
    for each (auto pair in Children_)
    {
        glPushMatrix(); // save current matrix
        Point position = pair.first->Position;
        glTranslated(position.X, position.Y, position.Z); // move child to its position
        glMultMatrixd(pair.second.Data); // add the child transformation
        pair.first->Render(); // render child
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
    for each (auto pair in Children_)
    {
        auto *found = pair.first->GetObjectByGLName(name);
        if (found != nullptr)
            return found;
    }
    return nullptr;
}

void VisualScene::AddChild(ref<VisualObject> child)
{
    for each (auto pair in Children_)
        if (pair.first == child)
            return;
    Children_.push_back(::std::make_pair(child, Transformation()));
}

Transformation &VisualScene::GetTransformation(int index)
{
    return Children_[index].second;
}

int VisualScene::IndexOf(ref<VisualObject> child)
{
    for (size_t i = 0; i < Children_.size(); ++i)
    {
        if (Children_[i].first == child)
            return static_cast<int>(i);
    }
    return -1;
}
