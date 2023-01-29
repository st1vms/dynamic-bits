cmake_minimum_required(VERSION 3.22.1)
project(dynamic-bits VERSION 1.3.0)

include_directories(include)

set(SOURCES
    src/dbits.c
    src/dpacket.c
)

add_executable(main main.c ${SOURCES})

target_link_libraries(main m)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG  "-g")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
