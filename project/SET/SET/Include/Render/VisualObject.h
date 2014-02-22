#pragma once

#include "../stdafx.h"

// this is the abstract base class of all visual object in the program, like card, deck, button...
class VisualObject abstract
{
public:
    virtual void Render() = 0; // visual object can render itself.
    virtual void OnMouseButtonDown() { } // visual object handle mouse button down.
};