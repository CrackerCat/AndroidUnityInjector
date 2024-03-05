cmake_minimum_required(VERSION 3.0)

project(utils)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} INTERFACE)

# fmt
# target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include)
# target_link_libraries(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/fmt/prebuilt/${ANDROID_ABI}/libfmt.a)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fmt)
target_link_libraries(${PROJECT_NAME} INTERFACE fmt)

# spdlog
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
target_link_libraries(${PROJECT_NAME} INTERFACE spdlog)

# magic_enum
add_subdirectory(magic_enum)
target_link_libraries(${PROJECT_NAME} INTERFACE magic_enum)

# 在 AndKittyInjector 中用到了一个于最新的版本不一致的xdl，所以还是保守使用依赖的xdl
# add_subdirectory(xdl)
# target_link_libraries(${PROJECT_NAME} INTERFACE xdl)

# debugbreak
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/debugbreak)

# rttr
set(RTTR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RTTR)
target_include_directories(${PROJECT_NAME} INTERFACE ${RTTR_DIR}/src)
target_include_directories(${PROJECT_NAME} INTERFACE ${RTTR_DIR}/prebuilt/${ANDROID_ABI}/src) # verison
target_link_libraries(${PROJECT_NAME} INTERFACE ${RTTR_DIR}/prebuilt/${ANDROID_ABI}/lib/librttr_core.a)

# LIEF
set(LIEF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/LIEF)
add_subdirectory(${LIEF_DIR})
target_link_libraries(${PROJECT_NAME} INTERFACE packLIEF)

# keystone
set(KEYSTONE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/keystone)
add_subdirectory(${KEYSTONE_DIR})
target_link_libraries(${PROJECT_NAME} INTERFACE keystone)

# capstone
set(CAPSTONE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/capstone)
add_subdirectory(${CAPSTONE_DIR})
target_link_libraries(${PROJECT_NAME} INTERFACE capstone)