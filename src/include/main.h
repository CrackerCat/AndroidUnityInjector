#if !defined(MAIN_CPP_H)
#define MAIN_CPP_H

#include "LuaLibrary.h"
#include "UnityResolve.hpp"
#include "bindings.h"
#include "debugbreak.h"
#include "dobby.h"

#include "Injector/KittyInjector.hpp"
#include "KittyMemoryMgr.hpp"
#include <algorithm>
#include <android/log.h>
#include <iostream>
#include <jni.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <xdl.h>

#include "capstone/capstone.h"
#include "keystone/keystone.h"

#include "log.h"

extern KittyInjector kitInjector;
extern KittyMemoryMgr kittyMemMgr;

#ifdef __linux__
#include <dlfcn.h>
#define GetModuleHandle dlopen
#endif
#ifdef DEBUG_PROJECT
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

// 测试的时候使用 （直接启动本地lua虚拟机同事使用测试代码及日志）
#define DEBUG_LOCAL false

// 兼容luabridge3对void*特化成 userdata 导致控制台不能直接输入数字视作void*的问题
#ifndef PTR
#define PTR uintptr_t
#endif

// 自定义输出二进制文件名称
#ifndef EXEC_NAME
#define EXEC_NAME "Injector"
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

// utils
std::string get_self_path();

#endif // MAIN_CPP_H