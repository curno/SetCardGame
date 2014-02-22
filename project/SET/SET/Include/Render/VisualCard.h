#pragma once

#include "VisualObject.h"
#include "../Model/Card.h"

class VisualCard : public VisualObject
{
    const Card &Card_;
public:
    VisualCard(const Card &card) : Card_(card) { }

    virtual void Render() override;
};