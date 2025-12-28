set(TARGET png_codec)

add_executable(${TARGET}
    src/png_codec.cpp
)

target_include_directories(${TARGET} PUBLIC src)