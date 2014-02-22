#pragma region

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