#include "bindings.h"

void reg_global(lua_State *L) {
    luabridge::getGlobalNamespace(L).addFunction(
        "clear", *[]() { system("clear"); });

    luabridge::getGlobalNamespace(L).addFunction(
        "exit", *[]() { exit(0); });

    luabridge::getGlobalNamespace(L).addFunction(
        "ls", *[]() { system("ls"); });

    luabridge::getGlobalNamespace(L).addFunction(
        "threadid", *[]() { std::cout << std::this_thread::get_id(); });

    DEBUG_PRINT("[*] luabridge bind global\n");
}