cmake_minimum_required(VERSION 3.5)

project(dobbyPack)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory(Dobby)
target_link_libraries(${PROJECT_NAME} INTERFACE dobby_static)
target_include_directories(${PROJECT_NAME} INTERFACE Dobby/include)