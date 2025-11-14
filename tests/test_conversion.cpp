#include <gtest/gtest.h>
#include "stevensMapLib.hpp"
#include <map>
#include <unordered_map>
#include <string>

// Test mapToVector function
TEST(ConversionTests, MapToVector_BasicMap)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 7}
    };

    auto result = stevensMapLib::mapToVector(testMap);

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0].first, "apple");
    EXPECT_EQ(result[0].second, 5);
}

TEST(ConversionTests, MapToVector_EmptyMap)
{
    std::map<int, double> emptyMap;
    auto result = stevensMapLib::mapToVector(emptyMap);
    EXPECT_TRUE(result.empty());
}

TEST(ConversionTests, MapToVector_UnorderedMap)
{
    std::unordered_map<int, std::string> testMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    auto result = stevensMapLib::mapToVector(testMap);
    EXPECT_EQ(result.size(), 3);
}

// Test mapToVecOfTuples function
TEST(ConversionTests, MapToVecOfTuples_BasicMap)
{
    std::map<int, std::string> testMap = {
        {1, "first"},
        {2, "second"},
        {3, "third"}
    };

    auto result = stevensMapLib::mapToVecOfTuples(testMap);

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(std::get<0>(result[0]), 1);
    EXPECT_EQ(std::get<1>(result[0]), "first");
}

TEST(ConversionTests, MapToVecOfTuples_EmptyMap)
{
    std::map<std::string, double> emptyMap;
    auto result = stevensMapLib::mapToVecOfTuples(emptyMap);
    EXPECT_TRUE(result.empty());
}

// Test mapToSortedVector function
TEST(ConversionTests, MapToSortedVector_SortByValueAscending)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 7},
        {"date", 1}
    };

    auto result = stevensMapLib::mapToSortedVector(testMap, "value", "ascending");

    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0].second, 1);
    EXPECT_EQ(result[1].second, 3);
    EXPECT_EQ(result[2].second, 5);
    EXPECT_EQ(result[3].second, 7);
}

TEST(ConversionTests, MapToSortedVector_SortByValueDescending)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 7}
    };

    auto result = stevensMapLib::mapToSortedVector(testMap, "value", "descending");

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0].second, 7);
    EXPECT_EQ(result[1].second, 5);
    EXPECT_EQ(result[2].second, 3);
}

TEST(ConversionTests, MapToSortedVector_SortByKeyAscending)
{
    std::map<int, std::string> testMap = {
        {3, "three"},
        {1, "one"},
        {2, "two"}
    };

    auto result = stevensMapLib::mapToSortedVector(testMap, "key", "ascending");

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0].first, 1);
    EXPECT_EQ(result[1].first, 2);
    EXPECT_EQ(result[2].first, 3);
}

TEST(ConversionTests, MapToSortedVector_SortByKeyDescending)
{
    std::map<int, std::string> testMap = {
        {3, "three"},
        {1, "one"},
        {2, "two"}
    };

    auto result = stevensMapLib::mapToSortedVector(testMap, "key", "descending");

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0].first, 3);
    EXPECT_EQ(result[1].first, 2);
    EXPECT_EQ(result[2].first, 1);
}

TEST(ConversionTests, MapToSortedVector_InvalidSortByParameter)
{
    std::map<int, int> testMap = {{1, 10}};
    EXPECT_THROW(
        stevensMapLib::mapToSortedVector(testMap, "invalid", "ascending"),
        std::invalid_argument
    );
}

TEST(ConversionTests, MapToSortedVector_InvalidOrderParameter)
{
    std::map<int, int> testMap = {{1, 10}};
    EXPECT_THROW(
        stevensMapLib::mapToSortedVector(testMap, "value", "invalid"),
        std::invalid_argument
    );
}
