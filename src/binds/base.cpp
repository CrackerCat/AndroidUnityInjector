#include "bindings.h"
#include "magic_enum.hpp"
#include <LIEF/LIEF.hpp>

using namespace LIEF;

void reg_base(lua_State *L) {

    // luabridge::getGlobalNamespace(L);

    fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "[*] luabridge bind base\n");
}