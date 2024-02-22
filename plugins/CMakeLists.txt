cmake_minimum_required(VERSION 3.5)

project(plugins)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory(Unity)
target_link_libraries(${PROJECT_NAME} INTERFACE UnityResover)

add_subdirectory(Utils)
target_link_libraries(${PROJECT_NAME} INTERFACE utils)

add_subdirectory(dobby)
target_link_libraries(${PROJECT_NAME} INTERFACE dobbyPack)