set(TARGET wav_codec)

add_library(${TARGET}
    wav_codec.cpp
        include/wav/enums.hpp
        include/wav/chunks.hpp
)

target_link_libraries(${TARGET} PUBLIC base_codec logger utils)
target_include_directories(${TARGET} PUBLIC include)
