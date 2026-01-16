cmake_minimum_required(VERSION 3.31)

set(TARGET TheKoder)
set(CMAKE_CXX_STANDARD 23)
set (CODECS
        png_codec
        bmp_codec
        wav_codec
)

set(COMPILE_OPTIONS -g -O0 -fno-omit-frame-pointer)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

include(cmake/zlibFetch.cmake)

project(${TARGET})

add_subdirectory(src/codecs)
add_subdirectory(src/logger)
add_subdirectory(src/utils)

add_executable(${TARGET}
        src/main.cpp
)

target_compile_options(${TARGET} PRIVATE ${COMPILE_OPTIONS})

target_link_libraries(${TARGET} PUBLIC
        utils
        base_codec
        logger
        ${CODECS}
)

target_include_directories(zlibstatic INTERFACE ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    enable_testing()
    add_subdirectory(test)
endif()