#pragma once

// this is the base class of the operation when a animation stops.
// this is a strategy class for Animation class.
class StopOperation
{
public:
    virtual ~StopOperation() { }
    virtual void operator()() = 0;
};

// this is the generic class of a StopOperation which takes a callable object as its behavior.
// Design Pattern: strategy
// the callable object is the strategy
template <typename Callable>
class GenericStopOperation : public StopOperation
{
    Callable Operation_;
public:
    GenericStopOperation(Callable callable) : Operation_(callable) { }
    virtual void operator()() { Operation_(); /* call strategy object */ }
};

// the convenient creation method for GenericStopOperation<...>
template <typename Callable>
ref<GenericStopOperation<Callable>> MakeGenericStopOperation(Callable callable)
{
    return ::std::make_shared<GenericStopOperation<Callable>>(callable);
}