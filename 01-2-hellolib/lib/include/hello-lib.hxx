#ifndef HELLO_LIB

#define HELLO_LIB

#ifdef WIN32

#ifdef PROJECTLIBRARY_EXPORTS
#define LIB_API __delspec(dllexport)
#else
#define LIB_API __delspec(dllimport)
#endif

#else
#define LIB_API
#endif

bool hello_world();

#endif
