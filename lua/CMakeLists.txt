cmake_minimum_required(VERSION 3.5)

project(lua_5_4_6)

file(GLOB_RECURSE LUA_SRC_FILES "src/*.c")

file(GLOB LUABRIDGE_TEST_LUA54_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    lua-5.4.4/src/*.c
    lua-5.4.4/src/*.h
    lua-5.4.4/src/*.hpp
    LuaLibrary.h
)