set(TARGET bmp_codec)

add_library(${TARGET}
        bmp_codec.cpp
        include/bmp.hpp
        include/utils.h
)

target_link_libraries(${TARGET} PUBLIC base_codec logger zlibstatic)
target_include_directories(${TARGET} PUBLIC include)
