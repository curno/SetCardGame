#pragma once

// The macro keywords for vc++ property.
#ifndef readwrite
#define readwrite(var) __declspec(property(get=Get##var, put=Set##var)) var
#endif

#ifndef readonly
#define readonly(var) __declspec(property(get=Get##var)) var
#endif

#ifndef writeonly
#define writeonly(var) __declspec(property(put=Set##var)) var
#endif

// Smart pointer
#define ref ::std::shared_ptr

#define PI (3.1415926535897932384626433832795)

#define IsType(Object, Type) (dynamic_cast<Type *>(Object) != nullptr)

template<typename T>
ref<T> shared_this(T *p) { return ::std::static_pointer_cast<T>(p->shared_from_this()); }
#define SHARED_THIS (shared_this(this))
