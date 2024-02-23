#include <main.h>

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    logd("------------------- JNI_OnLoad -------------------");
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) == JNI_OK) {
        logd("[*] GetEnv OK");
    }
    if (vm->AttachCurrentThread(&env, nullptr) == JNI_OK) {
        logd("[*] AttachCurrentThread OK");
    }
    g_jvm = vm;

    test();
    return JNI_VERSION_1_6;
}

void test() {

    void *handle = GetModuleHandle("libil2cpp.so", RTLD_LAZY);
    logd("libil2cpp.so handle: %p", handle);

    void *handle_xdl = xdl_open("libil2cpp.so", RTLD_LAZY);

    UnityResolve::Init(handle_xdl, UnityResolve::Mode::Il2Cpp);

    std::vector<UnityResolve::Assembly *> as = UnityResolve::assembly;
    std::for_each(as.begin(), as.end(), [&](auto item) {
    std::cout << item->name << "\tAssembly address: " << item->address << std::endl;
    logd("%p -> %s", item->address, item->name.c_str()); });

    // config lua binds

    // start lua repl
}