include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-3.2.28
)
set(SDL_STATIC ON CACHE BOOL "Build SDL3 static library" FORCE)
FetchContent_MakeAvailable(SDL3)
