#include "bindings.h"

void reg_global(lua_State *L) {

    luabridge::getGlobalNamespace(L)
        .addFunction(
            "clear", *[]() { system("clear"); })
        .addFunction(
            "exit", *[]() { exit(0); })
        .addFunction(
            "threadid", *[]() { std::cout << std::this_thread::get_id(); })
        .addFunction(
            "ls", *[]() { system("ls"); });

    console->info("[*] luabridge bind {}", "global");
}