#include <gtest/gtest.h>
#include "stevensMapLib.hpp"
#include <map>
#include <unordered_map>

// Test sumAllValues function
TEST(AggregationTests, SumAllValues_PositiveIntegers)
{
    std::map<std::string, int> testMap = {
        {"a", 5},
        {"b", 10},
        {"c", 15}
    };

    auto result = stevensMapLib::sumAllValues(testMap);
    EXPECT_EQ(result, 30);
}

TEST(AggregationTests, SumAllValues_WithInitialValue)
{
    std::map<std::string, int> testMap = {
        {"a", 5},
        {"b", 10}
    };

    auto result = stevensMapLib::sumAllValues(testMap, 100);
    EXPECT_EQ(result, 115);
}

TEST(AggregationTests, SumAllValues_NegativeValues)
{
    std::map<int, int> testMap = {
        {1, -5},
        {2, -10},
        {3, 3}
    };

    auto result = stevensMapLib::sumAllValues(testMap);
    EXPECT_EQ(result, -12);
}

TEST(AggregationTests, SumAllValues_DoubleValues)
{
    std::map<std::string, double> testMap = {
        {"a", 1.5},
        {"b", 2.5},
        {"c", 3.0}
    };

    auto result = stevensMapLib::sumAllValues(testMap);
    EXPECT_DOUBLE_EQ(result, 7.0);
}

TEST(AggregationTests, SumAllValues_EmptyMap)
{
    std::map<int, int> emptyMap;
    auto result = stevensMapLib::sumAllValues(emptyMap);
    EXPECT_EQ(result, 0);
}

// Test multiplyWithValues function
TEST(AggregationTests, MultiplyWithValues_PositiveFactor)
{
    std::map<std::string, double> testMap = {
        {"apple", 1.0},
        {"banana", 0.5},
        {"cherry", 2.0}
    };

    auto result = stevensMapLib::multiplyWithValues(testMap, 1.1);

    EXPECT_DOUBLE_EQ(result.at("apple"), 1.1);
    EXPECT_DOUBLE_EQ(result.at("banana"), 0.55);
    EXPECT_DOUBLE_EQ(result.at("cherry"), 2.2);
}

TEST(AggregationTests, MultiplyWithValues_IntegerValues)
{
    std::map<int, int> testMap = {
        {1, 10},
        {2, 20},
        {3, 30}
    };

    auto result = stevensMapLib::multiplyWithValues(testMap, 2.0);

    EXPECT_EQ(result.at(1), 20);
    EXPECT_EQ(result.at(2), 40);
    EXPECT_EQ(result.at(3), 60);
}

TEST(AggregationTests, MultiplyWithValues_ZeroFactor)
{
    std::map<std::string, int> testMap = {
        {"a", 100},
        {"b", 200}
    };

    auto result = stevensMapLib::multiplyWithValues(testMap, 0.0);

    EXPECT_EQ(result.at("a"), 0);
    EXPECT_EQ(result.at("b"), 0);
}

TEST(AggregationTests, MultiplyWithValues_NegativeFactor)
{
    std::map<int, int> testMap = {
        {1, 5},
        {2, -10}
    };

    auto result = stevensMapLib::multiplyWithValues(testMap, -2.0);

    EXPECT_EQ(result.at(1), -10);
    EXPECT_EQ(result.at(2), 20);
}

TEST(AggregationTests, MultiplyWithValues_EmptyMap)
{
    std::map<int, int> emptyMap;
    auto result = stevensMapLib::multiplyWithValues(emptyMap, 5.0);
    EXPECT_TRUE(result.empty());
}

// Test containsOnly function
TEST(AggregationTests, ContainsOnly_SingleMatchingKey)
{
    std::map<std::string, int> testMap = {{"apple", 5}};
    EXPECT_TRUE(stevensMapLib::containsOnly(testMap, std::string("apple")));
}

TEST(AggregationTests, ContainsOnly_MultipleKeys)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3}
    };
    EXPECT_FALSE(stevensMapLib::containsOnly(testMap, std::string("apple")));
}

TEST(AggregationTests, ContainsOnly_KeyNotPresent)
{
    std::map<std::string, int> testMap = {{"apple", 5}};
    EXPECT_FALSE(stevensMapLib::containsOnly(testMap, std::string("banana")));
}

TEST(AggregationTests, ContainsOnly_EmptyMap)
{
    std::map<int, int> emptyMap;
    EXPECT_FALSE(stevensMapLib::containsOnly(emptyMap, 1));
}
