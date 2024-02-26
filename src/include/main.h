#if !defined(MAIN_CPP_H)
#define MAIN_CPP_H

#include "LuaLibrary.h"
#include "UnityResolve.hpp"
#include "bindings.h"
#include "debugbreak.h"
#include "dobby.h"

#include <algorithm>
#include <android/log.h>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>
#include <xdl.h>

#ifdef __linux__
#include <dlfcn.h>
#define GetModuleHandle dlopen
#endif
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

#ifndef EXEC_NAME
#define EXEC_NAME "UnityInjector"
#endif

#define logd(...) __android_log_print(ANDROID_LOG_DEBUG, "ZZZ", __VA_ARGS__)

#define MAIN __attribute__((constructor))

#endif // MAIN_CPP_H