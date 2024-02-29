#include "bindings.h"
#include "magic_enum.hpp"
#include <LIEF/LIEF.hpp>

using namespace LIEF;

void reg_base(lua_State *L) {

    // printf("LIEF::ELF::VERSION: %d\n", magic_enum::enum_integer(ELF::VERSION::EV_CURRENT));

    DEBUG_PRINT("[*] luabridge bind base\n");
}