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
        stevensMapLib
        benchmark::benchmark
        benchmark::benchmark_main
)

target_include_directories(maplib_benchmarks PRIVATE ${CMAKE_SOURCE_DIR})
