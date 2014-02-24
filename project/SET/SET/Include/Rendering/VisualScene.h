#pragma once

#include "../stdafx.h"
#include "VisualObject.h"

// VisualScene is a VisualObject which contains multiple VisualObjects as children.
// Use Composite design pattern here.
class VisualScene : public VisualObject
{
    ::std::vector<ref<VisualObject>> Children_;
public:
    virtual void RenderContent() override;

    void AddChild(ref<VisualObject> child);

    virtual VisualObject *GetObjectByGLName(GLNameType name) override;
};