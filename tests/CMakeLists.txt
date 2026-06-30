add_executable(maplib_tests
    test_main.cpp
    test_conversion.cpp
    test_combination.cpp
    test_extraction.cpp
    test_modification.cpp
    test_aggregation.cpp
    test_string_operations.cpp
)

target_link_libraries(maplib_tests
    PRIVATE
        stevensMapLib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(maplib_tests)
