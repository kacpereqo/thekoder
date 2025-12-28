set(TARGET png_codec)

add_library(${TARGET}
        png_codec.cpp
)
target_link_libraries(${TARGET} PUBLIC base_codec)

target_include_directories(${TARGET} PUBLIC include)
