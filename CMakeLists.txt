cmake_minimum_required(VERSION 3.22.1)
project(dynamic-bits VERSION 1.3.0)

include_directories(include)

set(SOURCES
    src/dbits.c
    src/dpacket.c
    src/dserial.c
)

set(HEADER_FILES
    include/dbits.h
    include/dpacket.h
    include/dserial.h
    include/dtypes.h
)

add_library(dynamic-bits SHARED ${SOURCES} ${HEADER_FILES})

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    PUBLIC_HEADER "${HEADER_FILES}"
)

if (UNIX)
    set(CMAKE_INSTALL_PREFIX "/usr")
endif (UNIX)

install(TARGETS dynamic-bits
        CONFIGURATIONS Release
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
