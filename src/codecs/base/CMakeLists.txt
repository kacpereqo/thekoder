set(TARGET base_codec)

add_library(${TARGET} codec.cpp)

target_include_directories(${TARGET} PUBLIC include)
