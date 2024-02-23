#include "LuaLibrary.h"
#include "debugbreak.h"
#include "dobby.h"
#include "main.h"
#include <asm/signal.h>

void test_1(lua_State *L) {

    lua_getglobal(L, "print");

    std::string str = std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " + __FUNCTION__;
    lua_pushstring(L, str.c_str());

    lua_pushboolean(L, true);

    lua_getglobal(L, "print");
    lua_call(L, 3, 0);
}

void test_2(lua_State *L) {

    luaL_dostring(L, "print('test message')");

    const char *lua_str = R"(
        function sub(a, b)
            return sub_inner(a, b);
        end

        function sub_inner(a, b)
            print(debug.traceback())
            return a + b;
        end
    )";
    luaL_dostring(L, lua_str);

    lua_getglobal(L, "sub");
    lua_pushnumber(L, 10);
    lua_pushnumber(L, 5);
    lua_call(L, 2, 1);
    int result = lua_tonumber(L, -1);
    printf("result: %d\n", result);
    printf("lua_gettop: %p | lua_getstack: %p\n", lua_gettop(L), lua_getstack(L, 0, nullptr));
    lua_pop(L, 1);
    printf("lua_gettop: %p | lua_getstack: %p\n", lua_gettop(L), lua_getstack(L, 0, nullptr));
}

void test_3(lua_State *L) {
}

int main() {

    // raise(SIGSTOP);

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    test_1(L);

    test_2(L);

    return 0;
}
