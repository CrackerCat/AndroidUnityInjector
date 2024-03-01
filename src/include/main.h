#if !defined(MAIN_CPP_H)
#define MAIN_CPP_H

#include "LuaLibrary.h"
#include "UnityResolve.hpp"
#include "bindings.h"
#include "debugbreak.h"
#include "dobby.h"

#include <algorithm>
#include <android/log.h>
#include <fmt/color.h>
#include <iostream>
#include <jni.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <xdl.h>

#include "capstone/capstone.h"
#include "keystone/keystone.h"

#ifdef __linux__
#include <dlfcn.h>
#define GetModuleHandle dlopen
#endif
#ifdef DEBUG_PROJECT
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

#ifndef PTR
#define PTR uintptr_t
#endif

#ifndef EXEC_NAME
#define EXEC_NAME "UnityInjector"
#endif

#define logd(...) __android_log_print(ANDROID_LOG_DEBUG, "ZZZ", __VA_ARGS__)
#define loge(...) __android_log_print(ANDROID_LOG_ERROR, "ZZZ", __VA_ARGS__)

#if !defined(lua_printf)
#define lua_printf(s, l) fwrite((s), sizeof(char), (l), stdout)
#endif

#define MAIN __attribute__((constructor))

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved);

__attribute__((visibility("default"))) void startLuaVM();

#ifdef __cplusplus
}
#endif

#endif // MAIN_CPP_H