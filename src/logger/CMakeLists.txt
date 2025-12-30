set(TARGET logger)

add_library(${TARGET}
    src/logger.cpp
)

target_include_directories(${TARGET} PUBLIC include)
