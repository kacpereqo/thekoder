cmake_minimum_required(VERSION 3.31)

set(TARGET pngreader)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(CMAKE_CXX_STANDARD 23)

project(${TARGET})

include(cmake/fetch.cmake)
add_subdirectory (src/codecs)

add_executable(${TARGET}
        src/main.cpp
        src/utils.hpp
        src/utils.cpp
        src/logger.hpp
        src/codecs/base/include/codec.hpp
)

target_link_libraries(${TARGET} PUBLIC
        png_codec
        base_codec
)


target_include_directories(${TARGET} PRIVATE
        ${zlib_SOURCE_DIR}
        ${zlib_BINARY_DIR}
)

target_link_libraries(${TARGET}
        PRIVATE
        zlibstatic
)
