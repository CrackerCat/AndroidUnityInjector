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

    fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "[*] luabridge bind {}\n", "global");
}