#pragma once

#include "../stdafx.h"
#include "VisualObject.h"

// VisualScene is a VisualObject which contains multiple VisualObjects as children.
// Use Composite design pattern here.
class VisualScene : public VisualObject
{
    ::std::vector<ref<VisualObject>> Children_;
public:
    virtual void Render() override
    {
        // render each child
        for each (ref<VisualObject> child in Children_)
            child->Render();
    }

    void AddChild()
};