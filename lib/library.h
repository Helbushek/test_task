#pragma once

#ifdef _WIN32
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

extern "C" {

    // function 1
    DLL_EXPORT void function1(char* str);

    // function 2
    DLL_EXPORT int function2(const char* str);

    // function 3
    DLL_EXPORT bool function3(const char* str);
}