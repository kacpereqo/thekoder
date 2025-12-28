cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 23)
project(pngreader)

include(FetchContent)
add_subdirectory (src/codecs)

FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG        v1.3
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static zlib" FORCE)
FetchContent_MakeAvailable(zlib)


add_executable(pngreader
        src/main.cpp
        src/utils.hpp
        src/utils.cpp
        src/logger.hpp
        src/codecs/base/codec.hpp
)

target_link_libraries(pngreader
        PRIVATE
        zlibstatic     # zlib
)

#FetchContent_Declare(
#        SDL3
#        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#        GIT_TAG        release-3.2.28
#)
#set(SDL_STATIC ON CACHE BOOL "Build SDL3 static library" FORCE)
#FetchContent_MakeAvailable(SDL3)


#target_include_directories(pngreader
#        PRIVATE ${SDL3_SOURCE_DIR}/include
#)
