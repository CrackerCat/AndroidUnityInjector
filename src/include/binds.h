#ifndef MAIN_BINDS_H
#define MAIN_BINDS_H

#include "main.h"

void bind_libs(lua_State *L);

class xdl_bind;
void reg_xdl(lua_State *L);

class dobby_bind;
void reg_dobby(lua_State *L);

class unity_bind;
void reg_unity(lua_State *L);

// bind for UnityResolve
void reg_UnityResolve(lua_State *L);

#endif