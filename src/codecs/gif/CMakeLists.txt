set(TARGET gif_codec)

add_library(${TARGET}
        gif_codec.cpp
)

target_link_libraries(${TARGET} PUBLIC base_codec logger zlibstatic utils)


target_include_directories(${TARGET} PUBLIC include)
