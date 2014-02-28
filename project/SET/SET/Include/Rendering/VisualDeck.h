#pragma once

#include "VisualCard.h"
#include "Operation.h"
class VisualDeck : public VisualCard
{
    ref<Operation> Operation_;
public:
    VisualDeck() : VisualCard(nullptr, nullptr) { }
    ref<::Operation> readwrite(Operation);
    ref<::Operation> GetOperation() { return Operation_; }
    void SetOperation(ref<::Operation> operation) { Operation_ = operation; }
    virtual void OnMouseButtonDown() override
    {
        if (Operation_ != nullptr)
            (*Operation_)();
    }
};