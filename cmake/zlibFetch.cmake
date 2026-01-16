include(FetchContent)

FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG        v1.3
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static zlib" FORCE)
FetchContent_MakeAvailable(zlib)
