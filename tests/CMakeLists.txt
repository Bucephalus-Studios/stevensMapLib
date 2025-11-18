cmake_minimum_required(VERSION 3.14)
project(stevensMapLib_tests LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compile options for warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test executable
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
        GTest::gtest
        GTest::gtest_main
)

target_include_directories(maplib_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

enable_testing()
include(GoogleTest)
gtest_discover_tests(maplib_tests)
