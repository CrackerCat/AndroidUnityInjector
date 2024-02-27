cmake_minimum_required(VERSION 3.5)

project(uinjector)

add_definitions(-DEXEC_NAME="${PROJECT_NAME}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/include/*.h)

# binds
file(GLOB_RECURSE BINDS ${CMAKE_SOURCE_DIR}/src/binds/*.cpp)
list(APPEND SOURCES ${BINDS})

message(STATUS "SOURCES: ${SOURCES}")

if(MSVC)
    add_compile_options(/fdeclspec)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fdeclspec)
endif()

set(COMMON "-fPIC -w -o0 -g")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON} ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON}")

set(LIB_NAME ${PROJECT_NAME}_lib)
set(LIB_NAME_STATIC ${PROJECT_NAME}_static)

# gen common static lib
add_library(${LIB_NAME_STATIC} STATIC ${SOURCES})

target_include_directories(${LIB_NAME_STATIC} PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

add_subdirectory(plugins)
target_link_libraries(${LIB_NAME_STATIC} plugins)

add_subdirectory(lua)
target_link_libraries(${LIB_NAME_STATIC} lua_vm)

add_subdirectory(injector)
target_link_libraries(${LIB_NAME_STATIC} injector)

if(NOT ANDROID)
    find_library(log-lib log)
    target_link_libraries(${LIB_NAME_STATIC} ${log-lib})
else()
    target_link_libraries(${LIB_NAME_STATIC} log)
endif()

# test ( also below exec_program )
file(GLOB_RECURSE TESTS ${CMAKE_SOURCE_DIR}/src/test/*.cpp)
list(APPEND SOURCES ${TESTS})

file(GLOB EXEC_SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
message(STATUS "EXEC_SRC: ${EXEC_SRC}")

# gen executable (injector built in)
add_executable(${PROJECT_NAME} ${EXEC_SRC} ${TEST_SRC})
target_link_libraries(${PROJECT_NAME} ${LIB_NAME_STATIC})

# -fvisibility=default
target_compile_options(${PROJECT_NAME} PUBLIC -fvisibility=default)

set_property(GLOBAL PROPERTY JNI_OnLoad JNI_OnLoad)
set_property(GLOBAL PROPERTY startLuaVM startLuaVM)

# keep static void doREPL (lua_State *L)
# keep static int pmain (lua_State *L)
# set_target_properties(${PROJECT_NAME} PROPERTIES
# LINK_FLAGS "-Wl,--retain-symbols-file=${CMAKE_CURRENT_SOURCE_DIR}/symbols.txt"
# )