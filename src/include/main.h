#include "UnityResolve.hpp"

#include <algorithm>
#include <iostream>
#include <stdio.h>

#ifdef __linux__
#include <dlfcn.h>
#define GetModuleHandle dlopen
#endif

#include <thread>
#include <vector>

#include <android/log.h>

#include "debugbreak.h"

#include <xdl.h>

#define logd(...) __android_log_print(ANDROID_LOG_DEBUG, "ZZZ", __VA_ARGS__)

#define MAIN __attribute__((constructor))

#include <jni.h>

static JavaVM *g_jvm;
static JNIEnv *env;

void test();