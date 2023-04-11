cmake_minimum_required(VERSION 3.22.1)
project(test VERSION 1.0.0)

add_executable(test test.c)
target_link_libraries(test PUBLIC dynamic-bits m)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG  "-g")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
