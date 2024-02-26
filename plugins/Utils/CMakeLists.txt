cmake_minimum_required(VERSION 3.0)

project(utils)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory(magic_enum)
target_link_libraries(${PROJECT_NAME} INTERFACE magic_enum)

add_subdirectory(xdl)
target_link_libraries(${PROJECT_NAME} INTERFACE xdl)

# fmt
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include)
target_link_libraries(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/fmt/prebuilt/${ANDROID_ABI}/libfmt.a)

# debugbreak
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/debugbreak)

# rttr
set(RTTR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RTTR)
target_include_directories(${PROJECT_NAME} INTERFACE ${RTTR_DIR}/src)
target_include_directories(${PROJECT_NAME} INTERFACE ${RTTR_DIR}/prebuilt/${ANDROID_ABI}/src) # verison
target_link_libraries(${PROJECT_NAME} INTERFACE ${RTTR_DIR}/prebuilt/${ANDROID_ABI}/lib/librttr_core.a)