#include <benchmark/benchmark.h>
#include "stevensMapLib.hpp"
#include <map>
#include <unordered_map>
#include <random>

// Helper function to generate test data
template<typename MapType>
MapType generateTestMap(size_t size)
{
    MapType map;
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_int_distribution<> dis(1, 1000);

    for (size_t i = 0; i < size; ++i) {
        map[i] = dis(gen);
    }
    return map;
}

// Benchmark mapToVector
static void BM_MapToVector_Small(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(100);
    for (auto _ : state) {
        auto result = stevensMapLib::mapToVector(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MapToVector_Small);

static void BM_MapToVector_Medium(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::mapToVector(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MapToVector_Medium);

static void BM_MapToVector_Large(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(10000);
    for (auto _ : state) {
        auto result = stevensMapLib::mapToVector(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MapToVector_Large);

// Benchmark mapToVecOfTuples
static void BM_MapToVecOfTuples_Small(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(100);
    for (auto _ : state) {
        auto result = stevensMapLib::mapToVecOfTuples(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MapToVecOfTuples_Small);

static void BM_MapToVecOfTuples_Medium(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::mapToVecOfTuples(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MapToVecOfTuples_Medium);

// Benchmark mapToSortedVector
static void BM_MapToSortedVector_ByValue(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::mapToSortedVector(testMap, "value", "ascending");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MapToSortedVector_ByValue);

static void BM_MapToSortedVector_ByKey(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::mapToSortedVector(testMap, "key", "descending");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MapToSortedVector_ByKey);

// Compare std::map vs std::unordered_map
static void BM_MapToVector_StdMap(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::mapToVector(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MapToVector_StdMap);

static void BM_MapToVector_UnorderedMap(benchmark::State& state)
{
    auto testMap = generateTestMap<std::unordered_map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::mapToVector(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MapToVector_UnorderedMap);
