cmake_minimum_required(VERSION 3.14)
project(stevensMapLib_benchmarks LANGUAGES CXX)

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

# Fetch Google Benchmark
include(FetchContent)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

# Benchmark executable
add_executable(maplib_benchmarks
    benchmark_main.cpp
    benchmark_conversion.cpp
    benchmark_combination.cpp
    benchmark_extraction.cpp
    benchmark_modification.cpp
)

target_link_libraries(maplib_benchmarks
    PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
)

target_include_directories(maplib_benchmarks PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
