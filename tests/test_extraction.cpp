#include <gtest/gtest.h>
#include "stevensMapLib.hpp"
#include <map>
#include <unordered_map>
#include <algorithm>

// Test getKeyVector function
TEST(ExtractionTests, GetKeyVector_BasicMap)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 7}
    };

    auto result = stevensMapLib::getKeyVector(testMap);

    ASSERT_EQ(result.size(), 3);
    EXPECT_NE(std::find(result.begin(), result.end(), "apple"), result.end());
    EXPECT_NE(std::find(result.begin(), result.end(), "banana"), result.end());
    EXPECT_NE(std::find(result.begin(), result.end(), "cherry"), result.end());
}

TEST(ExtractionTests, GetKeyVector_EmptyMap)
{
    std::map<int, int> emptyMap;
    auto result = stevensMapLib::getKeyVector(emptyMap);
    EXPECT_TRUE(result.empty());
}

// Test getValueVector function
TEST(ExtractionTests, GetValueVector_BasicMap)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 7}
    };

    auto result = stevensMapLib::getValueVector(testMap);

    ASSERT_EQ(result.size(), 3);
    EXPECT_NE(std::find(result.begin(), result.end(), 5), result.end());
    EXPECT_NE(std::find(result.begin(), result.end(), 3), result.end());
    EXPECT_NE(std::find(result.begin(), result.end(), 7), result.end());
}

TEST(ExtractionTests, GetValueVector_EmptyMap)
{
    std::map<int, std::string> emptyMap;
    auto result = stevensMapLib::getValueVector(emptyMap);
    EXPECT_TRUE(result.empty());
}

// Test getFirstKey function
TEST(ExtractionTests, GetFirstKey_BasicMap)
{
    std::map<int, std::string> testMap = {
        {1, "first"},
        {2, "second"},
        {3, "third"}
    };

    auto result = stevensMapLib::getFirstKey(testMap);
    EXPECT_EQ(result, 1);  // std::map is ordered
}

// Test getFirstValue function
TEST(ExtractionTests, GetFirstValue_BasicMap)
{
    std::map<int, std::string> testMap = {
        {1, "first"},
        {2, "second"},
        {3, "third"}
    };

    auto result = stevensMapLib::getFirstValue(testMap);
    EXPECT_EQ(result, "first");
}

// Test getRandomKey function
TEST(ExtractionTests, GetRandomKey_BasicMap)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 7}
    };

    auto result = stevensMapLib::getRandomKey(testMap);
    EXPECT_TRUE(testMap.count(result));
}

TEST(ExtractionTests, GetRandomKey_EmptyMap)
{
    std::map<int, int> emptyMap;
    EXPECT_THROW(stevensMapLib::getRandomKey(emptyMap), std::invalid_argument);
}

TEST(ExtractionTests, GetRandomKey_SingleElement)
{
    std::map<std::string, int> singleMap = {{"only", 1}};
    auto result = stevensMapLib::getRandomKey(singleMap);
    EXPECT_EQ(result, "only");
}

// Test getRandomValue function
TEST(ExtractionTests, GetRandomValue_BasicMap)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 7}
    };

    auto result = stevensMapLib::getRandomValue(testMap);
    EXPECT_TRUE(result == 5 || result == 3 || result == 7);
}

TEST(ExtractionTests, GetRandomValue_EmptyMap)
{
    std::map<int, int> emptyMap;
    EXPECT_THROW(stevensMapLib::getRandomValue(emptyMap), std::invalid_argument);
}

// Test getPairWithMaxKey function
TEST(ExtractionTests, GetPairWithMaxKey_BasicMap)
{
    std::map<int, std::string> testMap = {
        {1, "low"},
        {5, "high"},
        {3, "medium"}
    };

    auto result = stevensMapLib::getPairWithMaxKey(testMap);
    EXPECT_EQ(result.first, 5);
    EXPECT_EQ(result.second, "high");
}

TEST(ExtractionTests, GetPairWithMaxKey_EmptyMap)
{
    std::map<int, int> emptyMap;
    EXPECT_THROW(stevensMapLib::getPairWithMaxKey(emptyMap), std::invalid_argument);
}

TEST(ExtractionTests, GetPairWithMaxKey_SingleElement)
{
    std::map<int, std::string> singleMap = {{42, "answer"}};
    auto result = stevensMapLib::getPairWithMaxKey(singleMap);
    EXPECT_EQ(result.first, 42);
}

// Test getPairWithMaxValue function
TEST(ExtractionTests, GetPairWithMaxValue_BasicMap)
{
    std::map<std::string, int> testMap = {
        {"low", 1},
        {"high", 100},
        {"medium", 50}
    };

    auto result = stevensMapLib::getPairWithMaxValue(testMap);
    EXPECT_EQ(result.first, "high");
    EXPECT_EQ(result.second, 100);
}

TEST(ExtractionTests, GetPairWithMaxValue_EmptyMap)
{
    std::map<int, int> emptyMap;
    EXPECT_THROW(stevensMapLib::getPairWithMaxValue(emptyMap), std::invalid_argument);
}

TEST(ExtractionTests, GetPairWithMaxValue_NegativeValues)
{
    std::map<std::string, int> testMap = {
        {"a", -10},
        {"b", -5},
        {"c", -20}
    };

    auto result = stevensMapLib::getPairWithMaxValue(testMap);
    EXPECT_EQ(result.first, "b");
    EXPECT_EQ(result.second, -5);
}

// Test getPairWithMinValue function
TEST(ExtractionTests, GetPairWithMinValue_BasicMap)
{
    std::map<std::string, int> testMap = {
        {"low", 1},
        {"high", 100},
        {"medium", 50}
    };

    auto result = stevensMapLib::getPairWithMinValue(testMap);
    EXPECT_EQ(result.first, "low");
    EXPECT_EQ(result.second, 1);
}

TEST(ExtractionTests, GetPairWithMinValue_EmptyMap)
{
    std::map<int, int> emptyMap;
    EXPECT_THROW(stevensMapLib::getPairWithMinValue(emptyMap), std::invalid_argument);
}

TEST(ExtractionTests, GetPairWithMinValue_NegativeValues)
{
    std::map<std::string, int> testMap = {
        {"a", -10},
        {"b", -5},
        {"c", -20}
    };

    auto result = stevensMapLib::getPairWithMinValue(testMap);
    EXPECT_EQ(result.first, "c");
    EXPECT_EQ(result.second, -20);
}
