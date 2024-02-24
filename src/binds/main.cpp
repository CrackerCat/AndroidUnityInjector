#include "binds.h"

void bind_libs(lua_State *L) {
    reg_xdl(L);
    reg_dobby(L);
    reg_UnityResolve(L);
}