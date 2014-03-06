#include "Include/stdafx.h"

#include "Include/Rendering/VisualScene.h"


void VisualScene::RenderContent()
{
    // render each child
    glMatrixMode(GL_MODELVIEW);
    for each (auto pair in Children_)
    {
        glPushMatrix(); // save current matrix
        Point position = pair.first->Position;
        glTranslated(position.X, position.Y, position.Z); // move child to its position
        glMultMatrixd(pair.second->Data); // add the child transformation
        pair.first->Render(); // render child
        glPopMatrix(); // restore
    }
}

void VisualScene::AddChild(ref<VisualObject> child)
{
    for each (auto pair in Children_)
        if (pair.first == child)
            return;
    Children_.push_back(::std::make_pair(child, new Transformation()));
}

Transformation &VisualScene::GetTransformation(int index)
{
    return *Children_[index].second;
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

VisualScene::~VisualScene()
{
    for each (auto pair in Children_)
        delete pair.second;
}

void VisualScene::RemoveChild(ref<VisualObject> child)
{
    for (auto i = Children_.begin(); i != Children_.end(); ++i)
    {
        if (i->first == child)
        {
            delete i->second;
            Children_.erase(i);
            return;
        }
    }
}

void VisualScene::RenderForPicking()
{
    // render each child
    glMatrixMode(GL_MODELVIEW);
    for each (auto pair in Children_)
    {
        glPushMatrix(); // save current matrix
        Point position = pair.first->Position;
        glTranslated(position.X, position.Y, position.Z); // move child to its position
        glMultMatrixd(pair.second->Data); // add the child transformation
        pair.first->RenderForPicking(); // render child
        glPopMatrix(); // restore
    }
}

VisualWidget *VisualScene::GetWidgetByGLName(GLNameType name)
{
    auto *retval = __super::GetWidgetByGLName(name);
    if (retval)
        return retval;
    // if not found, search in children
    for each (auto pair in Children_)
    {
        auto *found = pair.first->GetWidgetByGLName(name);
        if (found != nullptr)
            return found;
    }
    return nullptr;
}

VisualWidget * VisualScene::GetWidgetByViewportPosition(const CPoint &position)
{
    auto *retval = __super::GetWidgetByViewportPosition(position);
    if (retval)
        return retval;
    // if not found, search in children
    for each (auto pair in Children_)
    {
        auto *found = pair.first->GetWidgetByViewportPosition(position);
        if (found != nullptr)
            return found;
    }
    return nullptr;
}
