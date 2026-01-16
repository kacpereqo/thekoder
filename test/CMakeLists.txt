cmake_minimum_required(VERSION 3.14)

include(cmake/gtestFetch)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "codecs/*_test.cpp")

add_executable(unit_tests ${TEST_SOURCES})

target_link_libraries(unit_tests
        PRIVATE
        gtest_main
        ${CODECS}
)

include(GoogleTest)
gtest_discover_tests(unit_tests)