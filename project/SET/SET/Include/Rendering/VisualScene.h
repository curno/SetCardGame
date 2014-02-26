#pragma once

#include "../stdafx.h"
#include "VisualObject.h"

// VisualScene is a VisualObject which contains multiple VisualObjects as children.
// Use Composite design pattern here.
class VisualScene : public VisualObject
{
    // every child with its corresponding transformation
    ::std::vector<::std::pair<ref<VisualObject>, Transformation>> Children_;  
public:
    virtual void RenderContent() override;

    void AddChild(ref<VisualObject> child);
    
    using VisualObject::GetTransformation;
    Transformation &GetTransformation(int index);
    int IndexOf(ref<VisualObject> child);
    virtual VisualObject *GetObjectByGLName(GLNameType name) override;
};