#include <jni.h>
#include <main.h>

static JavaVM *g_jvm;
static JNIEnv *env;

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    logd("------------------- JNI_OnLoad -------------------");
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) == JNI_OK) {
        logd("[*] GetEnv OK");
    }
    if (vm->AttachCurrentThread(&env, nullptr) == JNI_OK) {
        logd("[*] AttachCurrentThread OK");
    }
    g_jvm = vm;

    return JNI_VERSION_1_6;
}