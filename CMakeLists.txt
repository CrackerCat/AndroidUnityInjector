cmake_minimum_required(VERSION 3.5)

project(UnityInjector)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

message(STATUS "SOURCES: ${SOURCES}")

if(MSVC)
    add_compile_options(/fdeclspec)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fdeclspec)
endif()

set(COMMON "-fPIC -w -Og -s")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON} ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON}")

set(LIB_NAME ${PROJECT_NAME}_lib)
set(LIB_NAME_STATIC ${PROJECT_NAME}_static)

add_library(${LIB_NAME_STATIC} STATIC ${SOURCES})

target_include_directories(${LIB_NAME_STATIC} PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

add_subdirectory(plugins)
target_link_libraries(${LIB_NAME_STATIC} plugins)

add_subdirectory(injector)
target_link_libraries(${LIB_NAME_STATIC} injector)

find_library(log-lib log)
target_link_libraries(${LIB_NAME_STATIC} ${log-lib})

add_library(${LIB_NAME} SHARED ${SOURCES})
target_link_libraries(${LIB_NAME} ${LIB_NAME_STATIC})

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/inject.cpp)
target_link_libraries(${PROJECT_NAME} ${LIB_NAME_STATIC})
