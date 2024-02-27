#include <iostream>
#include <jni.h>
#include <main.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

static JavaVM *g_jvm;
static JNIEnv *env;
static std::thread *g_thread = NULL;

static int report(lua_State *L, int status) {
    if (status != LUA_OK) {
        const char *msg = lua_tostring(L, -1);
        lua_writestringerror("%s\n", msg);
        lua_pop(L, 1);
    }
    return status;
}

void repl_socket(lua_State *L) {

    int serverSocket, clientSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8024);

    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char buffer[1024] = {0};
    int valread;
    std::string input;
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        printf("exec > ");
        fflush(stdout);
        valread = read(clientSocket, buffer, sizeof(buffer));
        if (valread == 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        } else if (valread == -1) {
            perror("read");
            break;
        } else {
            input = buffer;
            if (input == "exit" || input == "q") {
                std::cout << "Client requested exit." << std::endl;
                break;
            }
            int status = luaL_dostring(L, input.c_str());
            if (status == 1)
                report(L, status);
        }
    }

    // close(clientSocket);
    // close(serverSocket);
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

    // repl_socket(L);

    // lua_close(L);
}