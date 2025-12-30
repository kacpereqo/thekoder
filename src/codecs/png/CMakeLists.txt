set(TARGET png_codec)

add_library(${TARGET}
        png_codec.cpp
)

target_link_libraries(${TARGET} PUBLIC base_codec logger zlibstatic)


target_include_directories(${TARGET} PUBLIC include)
