set(TARGET base_codec)

add_library(${TARGET} codec.cpp
        include/colors.hpp)

target_include_directories(${TARGET} PUBLIC include)
