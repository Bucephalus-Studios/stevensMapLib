#include <gtest/gtest.h>
#include "stevensMapLib.hpp"
#include <map>
#include <unordered_map>

// Test erase function
TEST(ModificationTests, Erase_MultipleKeys)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 7},
        {"date", 2}
    };

    std::vector<std::string> toErase = {"apple", "cherry"};
    auto result = stevensMapLib::erase(testMap, toErase);

    EXPECT_EQ(result.size(), 2);
    EXPECT_FALSE(result.count("apple"));
    EXPECT_TRUE(result.count("banana"));
    EXPECT_FALSE(result.count("cherry"));
    EXPECT_TRUE(result.count("date"));
}

TEST(ModificationTests, Erase_EmptyVector)
{
    std::map<int, int> testMap = {{1, 10}, {2, 20}};
    std::vector<int> toErase;

    auto result = stevensMapLib::erase(testMap, toErase);
    EXPECT_EQ(result.size(), 2);
}

TEST(ModificationTests, Erase_NonExistentKeys)
{
    std::map<int, int> testMap = {{1, 10}, {2, 20}};
    std::vector<int> toErase = {3, 4, 5};

    auto result = stevensMapLib::erase(testMap, toErase);
    EXPECT_EQ(result.size(), 2);
}

TEST(ModificationTests, Erase_AllKeys)
{
    std::map<std::string, int> testMap = {{"a", 1}, {"b", 2}};
    std::vector<std::string> toErase = {"a", "b"};

    auto result = stevensMapLib::erase(testMap, toErase);
    EXPECT_TRUE(result.empty());
}

// Test setNegativeValuesToZero function
TEST(ModificationTests, SetNegativeValuesToZero_MixedValues)
{
    std::map<std::string, int> testMap = {
        {"positive", 10},
        {"negative", -5},
        {"zero", 0},
        {"more_negative", -100}
    };

    stevensMapLib::setNegativeValuesToZero(testMap);

    EXPECT_EQ(testMap.at("positive"), 10);
    EXPECT_EQ(testMap.at("negative"), 0);
    EXPECT_EQ(testMap.at("zero"), 0);
    EXPECT_EQ(testMap.at("more_negative"), 0);
}

TEST(ModificationTests, SetNegativeValuesToZero_AllPositive)
{
    std::map<int, double> testMap = {{1, 1.5}, {2, 2.5}, {3, 3.5}};
    stevensMapLib::setNegativeValuesToZero(testMap);

    EXPECT_DOUBLE_EQ(testMap.at(1), 1.5);
    EXPECT_DOUBLE_EQ(testMap.at(2), 2.5);
    EXPECT_DOUBLE_EQ(testMap.at(3), 3.5);
}

TEST(ModificationTests, SetNegativeValuesToZero_AllNegative)
{
    std::map<int, int> testMap = {{1, -1}, {2, -2}, {3, -3}};
    stevensMapLib::setNegativeValuesToZero(testMap);

    EXPECT_EQ(testMap.at(1), 0);
    EXPECT_EQ(testMap.at(2), 0);
    EXPECT_EQ(testMap.at(3), 0);
}

TEST(ModificationTests, SetNegativeValuesToZero_EmptyMap)
{
    std::map<int, int> testMap;
    stevensMapLib::setNegativeValuesToZero(testMap);
    EXPECT_TRUE(testMap.empty());
}

// Test popRandom function
TEST(ModificationTests, PopRandom_BasicMap)
{
    std::map<std::string, int> testMap = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 7}
    };

    size_t originalSize = testMap.size();
    auto result = stevensMapLib::popRandom(testMap);

    EXPECT_EQ(testMap.size(), originalSize - 1);
    EXPECT_FALSE(testMap.count(result.first));
}

TEST(ModificationTests, PopRandom_EmptyMap)
{
    std::map<int, int> emptyMap;
    EXPECT_THROW(stevensMapLib::popRandom(emptyMap), std::invalid_argument);
}

TEST(ModificationTests, PopRandom_SingleElement)
{
    std::map<std::string, int> singleMap = {{"only", 42}};
    auto result = stevensMapLib::popRandom(singleMap);

    EXPECT_EQ(result.first, "only");
    EXPECT_EQ(result.second, 42);
    EXPECT_TRUE(singleMap.empty());
}

// Test insertNumericRangeAsStringKeys function
TEST(ModificationTests, InsertNumericRangeAsStringKeys_PositiveRange)
{
    std::map<std::string, int> testMap;
    stevensMapLib::insertNumericRangeAsStringKeys(testMap, 1, 5);

    EXPECT_EQ(testMap.size(), 5);
    EXPECT_TRUE(testMap.count("1"));
    EXPECT_TRUE(testMap.count("2"));
    EXPECT_TRUE(testMap.count("3"));
    EXPECT_TRUE(testMap.count("4"));
    EXPECT_TRUE(testMap.count("5"));
}

TEST(ModificationTests, InsertNumericRangeAsStringKeys_NegativeRange)
{
    std::map<std::string, int> testMap;
    stevensMapLib::insertNumericRangeAsStringKeys(testMap, -2, 2);

    EXPECT_EQ(testMap.size(), 5);
    EXPECT_TRUE(testMap.count("-2"));
    EXPECT_TRUE(testMap.count("-1"));
    EXPECT_TRUE(testMap.count("0"));
    EXPECT_TRUE(testMap.count("1"));
    EXPECT_TRUE(testMap.count("2"));
}

TEST(ModificationTests, InsertNumericRangeAsStringKeys_SingleValue)
{
    std::map<std::string, int> testMap;
    stevensMapLib::insertNumericRangeAsStringKeys(testMap, 5, 5);

    EXPECT_EQ(testMap.size(), 1);
    EXPECT_TRUE(testMap.count("5"));
}

TEST(ModificationTests, InsertNumericRangeAsStringKeys_ExistingMap)
{
    std::map<std::string, int> testMap = {{"existing", 100}};
    stevensMapLib::insertNumericRangeAsStringKeys(testMap, 1, 3);

    EXPECT_EQ(testMap.size(), 4);
    EXPECT_TRUE(testMap.count("existing"));
    EXPECT_TRUE(testMap.count("1"));
    EXPECT_TRUE(testMap.count("2"));
    EXPECT_TRUE(testMap.count("3"));
}
