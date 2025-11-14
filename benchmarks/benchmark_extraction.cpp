#include <benchmark/benchmark.h>
#include "stevensMapLib.hpp"
#include <map>
#include <random>

// Helper function to generate test data
template<typename MapType>
MapType generateTestMap(size_t size)
{
    MapType map;
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(1, 1000);

    for (size_t i = 0; i < size; ++i) {
        map[i] = dis(gen);
    }
    return map;
}

// Benchmark getKeyVector
static void BM_GetKeyVector_Small(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(100);
    for (auto _ : state) {
        auto result = stevensMapLib::getKeyVector(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetKeyVector_Small);

static void BM_GetKeyVector_Large(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(10000);
    for (auto _ : state) {
        auto result = stevensMapLib::getKeyVector(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetKeyVector_Large);

// Benchmark getValueVector
static void BM_GetValueVector_Small(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(100);
    for (auto _ : state) {
        auto result = stevensMapLib::getValueVector(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetValueVector_Small);

static void BM_GetValueVector_Large(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(10000);
    for (auto _ : state) {
        auto result = stevensMapLib::getValueVector(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetValueVector_Large);

// Benchmark getPairWithMaxValue
static void BM_GetPairWithMaxValue_Small(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(100);
    for (auto _ : state) {
        auto result = stevensMapLib::getPairWithMaxValue(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetPairWithMaxValue_Small);

static void BM_GetPairWithMaxValue_Large(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(10000);
    for (auto _ : state) {
        auto result = stevensMapLib::getPairWithMaxValue(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetPairWithMaxValue_Large);

// Benchmark getPairWithMinValue
static void BM_GetPairWithMinValue_Small(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(100);
    for (auto _ : state) {
        auto result = stevensMapLib::getPairWithMinValue(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetPairWithMinValue_Small);

static void BM_GetPairWithMinValue_Large(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(10000);
    for (auto _ : state) {
        auto result = stevensMapLib::getPairWithMinValue(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetPairWithMinValue_Large);

// Benchmark getPairWithMaxKey
static void BM_GetPairWithMaxKey(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::getPairWithMaxKey(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetPairWithMaxKey);

// Benchmark getRandomKey
static void BM_GetRandomKey(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::getRandomKey(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetRandomKey);

// Benchmark getRandomValue
static void BM_GetRandomValue(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::getRandomValue(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetRandomValue);

// Benchmark getFirstKey
static void BM_GetFirstKey(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::getFirstKey(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetFirstKey);

// Benchmark getFirstValue
static void BM_GetFirstValue(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::getFirstValue(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetFirstValue);
