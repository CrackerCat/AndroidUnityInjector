cmake_minimum_required(VERSION 3.5)

project(lua_vm)

file(GLOB LUA_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/lua-5.4.6/*.c)

add_library(${PROJECT_NAME}
    STATIC
    ${LUA_SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lua-5.4.6)

# keep static void doREPL (lua_State *L)
# keep static int pmain (lua_State *L)
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_FLAGS "-Wl,--retain-symbols-file=${CMAKE_CURRENT_SOURCE_DIR}/symbols.txt"
)

# 添加编译属性 保留调试符号
target_compile_options(${PROJECT_NAME} PRIVATE -g)

# or using LuaJIT
# ......