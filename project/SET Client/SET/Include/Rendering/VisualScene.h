#pragma once

#include "../stdafx.h"
#include "VisualObject.h"

class VisualWidget;
// VisualScene is a VisualObject which contains multiple VisualObjects as children.
// Use Composite design pattern here.
class VisualScene : public VisualObject
{
    // every child with its corresponding transformation
    ::std::vector<::std::pair<ref<VisualObject>, Transformation *>> Children_;  
public:
    virtual ~VisualScene();
    virtual void RenderForPicking() override;
    void AddChild(ref<VisualObject> child);
    void RemoveChild(ref<VisualObject> child);
    using VisualObject::GetTransformation;
    Transformation &GetTransformation(int index);
    int IndexOf(ref<VisualObject> child);
    virtual VisualWidget *GetWidgetByGLName(GLNameType name) override;
    virtual VisualWidget *GetWidgetByViewportPosition(const CPoint &position) override;
protected:
    virtual void RenderContent() override;
};