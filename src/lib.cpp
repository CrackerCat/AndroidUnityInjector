#include <jni.h>
#include <main.h>

static int report(lua_State *L, int status) {
    if (status != LUA_OK) {
        const char *msg = lua_tostring(L, -1);
        lua_writestringerror("%s\n", msg);
        lua_pop(L, 1); /* remove message */
    }
    return status;
}

static void repl(lua_State *L) {
    std::string input;
    while (true) {
        printf("exec > ");
        std::getline(std::cin, input);
        if (input == "exit" || input == "q")
            break;
        int status = luaL_dostring(L, input.c_str());
        if (status == 1)
            report(L, status);
    }
}

static JavaVM *g_jvm;
static JNIEnv *env;
static std::thread *g_thread = NULL;
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    if (vm == nullptr)
        return JNI_VERSION_1_6;
    logd("------------------- JNI_OnLoad -------------------");
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) == JNI_OK) {
        logd("[*] GetEnv OK");
    }
    if (vm->AttachCurrentThread(&env, nullptr) == JNI_OK) {
        logd("[*] AttachCurrentThread OK");
    }
    g_jvm = vm;

    g_thread = new std::thread([]() {
        startLuaVM();
    });
    g_thread->detach();

    return JNI_VERSION_1_6;
}

void startLuaVM() {
    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    bind_libs(L);

    // test(L);

    repl(L);

    lua_close(L);
}