#pragma once

#ifdef __unix__
#define OS_LINUX
#elif defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
#else
#error Unknown OS
#endif

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#define DLL_IMPORT __attribute__ ((visibility ("hidden")))
#else 
#error Unknown Compiler
#endif

#define WAIT_SLEEP_TIME std::chrono::milliseconds(1)
