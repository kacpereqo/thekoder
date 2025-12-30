cmake_minimum_required(VERSION 3.31)

set(TARGET pngreader)

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(${TARGET})

include(cmake/fetch.cmake)

add_subdirectory(src/codecs)
add_subdirectory(src/logger)

add_executable(${TARGET}
        src/main.cpp
        src/utils.hpp
        src/utils.cpp
)

target_compile_options(${TARGET} PRIVATE -g -O0 -fno-omit-frame-pointer)

target_link_libraries(${TARGET} PUBLIC
        png_codec
        base_codec
        logger
)


target_include_directories(zlibstatic INTERFACE ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})


target_link_libraries(${TARGET} PUBLIC zlib)

