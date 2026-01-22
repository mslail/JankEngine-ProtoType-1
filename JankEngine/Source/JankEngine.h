#pragma once


#ifdef WIN32

#include <Windows.h>

#endif

#ifdef BUILD_DLL
#define JANK_ENGINE_API __declspec(dllexport)
#else
#define JANK_ENGINE_API __declspec(dllimport)
#endif