cmake_minimum_required(VERSION 3.16)
project(stevensMapLib LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(stevensMathLib
    GIT_REPOSITORY https://github.com/Bucephalus-Studios/stevensMathLib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(stevensMathLib)

add_library(stevensMapLib INTERFACE)
add_library(stevensMapLib::stevensMapLib ALIAS stevensMapLib)

target_include_directories(stevensMapLib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(stevensMapLib INTERFACE cxx_std_17)
target_link_libraries(stevensMapLib INTERFACE stevensMathLib)

option(STEVENSMAPLIB_BUILD_TESTS "Build tests" OFF)
option(STEVENSMAPLIB_BUILD_BENCHMARKS "Build benchmarks" OFF)

if(STEVENSMAPLIB_BUILD_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    add_subdirectory(tests)
endif()

if(STEVENSMAPLIB_BUILD_BENCHMARKS)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    FetchContent_MakeAvailable(benchmark)
    add_subdirectory(benchmarks)
endif()
