#include <benchmark/benchmark.h>
#include "stevensMapLib.hpp"
#include <map>
#include <random>

// Helper function to generate test data
template<typename MapType>
MapType generateTestMap(size_t size, int seed = 42)
{
    MapType map;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(1, 1000);

    for (size_t i = 0; i < size; ++i) {
        map[i] = dis(gen);
    }
    return map;
}

// Benchmark addMaps
static void BM_AddMaps_Values_NoOmit(benchmark::State& state)
{
    auto mapA = generateTestMap<std::map<int, int>>(500);
    auto mapB = generateTestMap<std::map<int, int>>(500, 123);

    for (auto _ : state) {
        auto result = stevensMapLib::addMaps(mapA, mapB, "values", false);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_AddMaps_Values_NoOmit);

static void BM_AddMaps_Values_WithOmit(benchmark::State& state)
{
    auto mapA = generateTestMap<std::map<int, int>>(500);
    auto mapB = generateTestMap<std::map<int, int>>(500, 123);

    for (auto _ : state) {
        auto result = stevensMapLib::addMaps(mapA, mapB, "values", true);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_AddMaps_Values_WithOmit);

static void BM_AddMaps_KeysAndValues(benchmark::State& state)
{
    std::map<std::string, int> mapA, mapB;
    for (int i = 0; i < 500; ++i) {
        mapA[std::string("key") + std::to_string(i)] = i;
        mapB[std::string("key") + std::to_string(i)] = i * 2;
    }

    for (auto _ : state) {
        auto result = stevensMapLib::addMaps(mapA, mapB, std::string("keys and values"), false);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_AddMaps_KeysAndValues);

// Benchmark combineMaps
static void BM_CombineMaps_NoOverwrite(benchmark::State& state)
{
    auto source = generateTestMap<std::map<int, int>>(500);
    auto dest = generateTestMap<std::map<int, int>>(500, 123);

    for (auto _ : state) {
        auto result = stevensMapLib::combineMaps(source, dest, false);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CombineMaps_NoOverwrite);

static void BM_CombineMaps_WithOverwrite(benchmark::State& state)
{
    auto source = generateTestMap<std::map<int, int>>(500);
    auto dest = generateTestMap<std::map<int, int>>(500, 123);

    for (auto _ : state) {
        auto result = stevensMapLib::combineMaps(source, dest, true);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CombineMaps_WithOverwrite);

// Benchmark with different overlap percentages
static void BM_AddMaps_LowOverlap(benchmark::State& state)
{
    std::map<int, int> mapA, mapB;
    for (int i = 0; i < 1000; ++i) {
        mapA[i] = i;
    }
    for (int i = 900; i < 1900; ++i) {  // 10% overlap
        mapB[i] = i;
    }

    for (auto _ : state) {
        auto result = stevensMapLib::addMaps(mapA, mapB, "values", false);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_AddMaps_LowOverlap);

static void BM_AddMaps_HighOverlap(benchmark::State& state)
{
    std::map<int, int> mapA, mapB;
    for (int i = 0; i < 1000; ++i) {
        mapA[i] = i;
    }
    for (int i = 100; i < 1100; ++i) {  // 90% overlap
        mapB[i] = i;
    }

    for (auto _ : state) {
        auto result = stevensMapLib::addMaps(mapA, mapB, "values", false);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_AddMaps_HighOverlap);
