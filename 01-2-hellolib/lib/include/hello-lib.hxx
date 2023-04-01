#ifndef HELLO_LIB

#define HELLO_LIB

#ifdef WIN32

#define LIB_API __declspec(dllexport)

#else
#define LIB_API
#endif

LIB_API bool hello_world();

#endif
