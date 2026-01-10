cmake_minimum_required(VERSION 3.31)

set(TARGET pngreader)

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(SDL3 REQUIRED)

project(${TARGET})

include(cmake/fetch.cmake)

add_subdirectory(src/codecs)
add_subdirectory(src/logger)
add_subdirectory(src/utils)

add_executable(${TARGET}
        src/main.cpp
        src/codecs/wav/include/wav_codec.hpp
        src/codecs/wav/wav_codec.cpp
)

target_compile_options(${TARGET} PRIVATE -g -O0 -fno-omit-frame-pointer)

target_link_libraries(${TARGET} PUBLIC
        utils
        png_codec
        bmp_codec
        base_codec
        logger
)

target_include_directories(zlibstatic INTERFACE ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
target_link_libraries(${TARGET} PRIVATE SDL3::SDL3-static)

