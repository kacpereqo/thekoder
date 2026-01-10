set(TARGET bmp_codec)

add_library(${TARGET}
        bmp_codec.cpp
        chunks.cpp
        include/enums.hpp
)

target_link_libraries(${TARGET} PUBLIC base_codec logger utils zlibstatic)
target_include_directories(${TARGET} PUBLIC include)
