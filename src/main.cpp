#include "main.h"
#include "test.h"

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

int main() {

    lua_State *L = nullptr;
    // raise(SIGSTOP);

    L = luaL_newstate();

    luaL_openlibs(L);

    bind_libs(L);

    // test_1(L);

    // test_2(L);

    // test_3(L);

    repl(L);

    lua_close(L);

    return 0;
}
