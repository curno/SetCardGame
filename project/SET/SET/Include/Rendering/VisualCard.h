#pragma once

#include "VisualObject.h"
#include "../Model/Card.h"

class VisualCard : public VisualObject
{
    static const int DefaultWidth = 100;
    static const int DefaultHeight = 200;
    CardRef Card_;
public:
    VisualCard(const CardRef card) : Card_(card) 
    { 
        Size = CSize(DefaultWidth, DefaultHeight);
    }

    virtual void RenderContent() override;
    
    void SetCard(const CardRef card) { Card_ = card; }
};