#include "LuaLibrary.h"
#include "debugbreak.h"
#include "dobby.h"
#include "main.h"
#include <asm/signal.h>

#include "test.h"

int main() {

    // raise(SIGSTOP);

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // test_1(L);

    // test_2(L);

    test_3(L);

    test_4(L);

    lua_close(L);

    return 0;
}
