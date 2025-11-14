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
    std::uniform_int_distribution<> dis(-100, 100);

    for (size_t i = 0; i < size; ++i) {
        map[i] = dis(gen);
    }
    return map;
}

// Helper function to generate test data for string-keyed maps
template<typename V>
std::map<std::string, V> generateStringKeyMap(size_t size)
{
    std::map<std::string, V> map;
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(-100, 100);

    for (size_t i = 0; i < size; ++i) {
        map[std::to_string(i)] = dis(gen);
    }
    return map;
}

// Benchmark sumAllValues
static void BM_SumAllValues_Small(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(100);
    for (auto _ : state) {
        auto result = stevensMapLib::sumAllValues(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_SumAllValues_Small);

static void BM_SumAllValues_Large(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(10000);
    for (auto _ : state) {
        auto result = stevensMapLib::sumAllValues(testMap);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_SumAllValues_Large);

// Benchmark multiplyWithValues
static void BM_MultiplyWithValues_Small(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(100);
    for (auto _ : state) {
        auto result = stevensMapLib::multiplyWithValues(testMap, 1.5);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MultiplyWithValues_Small);

static void BM_MultiplyWithValues_Large(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(10000);
    for (auto _ : state) {
        auto result = stevensMapLib::multiplyWithValues(testMap, 1.5);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MultiplyWithValues_Large);

// Benchmark setNegativeValuesToZero
static void BM_SetNegativeValuesToZero_Small(benchmark::State& state)
{
    for (auto _ : state) {
        state.PauseTiming();
        auto testMap = generateTestMap<std::map<int, int>>(100);
        state.ResumeTiming();

        stevensMapLib::setNegativeValuesToZero(testMap);
        benchmark::DoNotOptimize(testMap);
    }
}
BENCHMARK(BM_SetNegativeValuesToZero_Small);

static void BM_SetNegativeValuesToZero_Large(benchmark::State& state)
{
    for (auto _ : state) {
        state.PauseTiming();
        auto testMap = generateTestMap<std::map<int, int>>(10000);
        state.ResumeTiming();

        stevensMapLib::setNegativeValuesToZero(testMap);
        benchmark::DoNotOptimize(testMap);
    }
}
BENCHMARK(BM_SetNegativeValuesToZero_Large);

// Benchmark erase
static void BM_Erase_FewKeys(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    std::vector<int> keysToErase = {10, 20, 30, 40, 50};

    for (auto _ : state) {
        auto result = stevensMapLib::erase(testMap, keysToErase);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Erase_FewKeys);

static void BM_Erase_ManyKeys(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(1000);
    std::vector<int> keysToErase;
    for (int i = 0; i < 500; ++i) {
        keysToErase.push_back(i);
    }

    for (auto _ : state) {
        auto result = stevensMapLib::erase(testMap, keysToErase);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Erase_ManyKeys);

// Benchmark containsOnly
static void BM_ContainsOnly_True(benchmark::State& state)
{
    std::map<int, int> testMap = {{42, 100}};
    for (auto _ : state) {
        auto result = stevensMapLib::containsOnly(testMap, 42);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ContainsOnly_True);

static void BM_ContainsOnly_False(benchmark::State& state)
{
    auto testMap = generateTestMap<std::map<int, int>>(100);
    for (auto _ : state) {
        auto result = stevensMapLib::containsOnly(testMap, 42);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ContainsOnly_False);

// Benchmark string operations
static void BM_GetPairsWhereKeysStartWith(benchmark::State& state)
{
    std::map<std::string, int> testMap;
    for (int i = 0; i < 1000; ++i) {
        if (i % 3 == 0) {
            testMap[std::string("prefix_") + std::to_string(i)] = i;
        } else {
            testMap[std::string("other_") + std::to_string(i)] = i;
        }
    }

    for (auto _ : state) {
        auto result = stevensMapLib::getPairsWhereKeysStartWith(testMap, std::string("prefix_"));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_GetPairsWhereKeysStartWith);

static void BM_ErasePairsWhereKeysStartWith(benchmark::State& state)
{
    std::map<std::string, int> testMap;
    for (int i = 0; i < 1000; ++i) {
        if (i % 3 == 0) {
            testMap[std::string("prefix_") + std::to_string(i)] = i;
        } else {
            testMap[std::string("other_") + std::to_string(i)] = i;
        }
    }

    for (auto _ : state) {
        auto result = stevensMapLib::erasePairsWhereKeysStartWith(testMap, std::string("prefix_"));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ErasePairsWhereKeysStartWith);

static void BM_EraseStringFromKeys(benchmark::State& state)
{
    std::map<std::string, int> testMap;
    for (int i = 0; i < 500; ++i) {
        testMap[std::string("style:element") + std::to_string(i)] = i;
    }

    for (auto _ : state) {
        auto result = stevensMapLib::eraseStringFromKeys(testMap, std::string("style:"));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_EraseStringFromKeys);

static void BM_CreateUniqueKeyString_NoConflict(benchmark::State& state)
{
    auto testMap = generateStringKeyMap<int>(1000);
    for (auto _ : state) {
        auto result = stevensMapLib::createUniqueKeyString(testMap, std::string("unique_key"));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CreateUniqueKeyString_NoConflict);

static void BM_CreateUniqueKeyString_WithConflict(benchmark::State& state)
{
    std::map<std::string, int> testMap;
    testMap[std::string("key")] = 1;
    for (int i = 0; i < 100; ++i) {
        testMap[std::string("key") + std::to_string(i)] = i;
    }

    for (auto _ : state) {
        auto result = stevensMapLib::createUniqueKeyString(testMap, std::string("key"));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CreateUniqueKeyString_WithConflict);
