#include <gtest/gtest.h>
#include "stevensMapLib.hpp"
#include <map>
#include <string>

// Test getPairsWhereKeysStartWith function
TEST(StringOperationTests, GetPairsWhereKeysStartWith_MatchingKeys)
{
    std::map<std::string, std::string> testMap = {
        {"style:textColor", "red"},
        {"style:backgroundColor", "black"},
        {"sectionName", "header"},
        {"style:fontSize", "12px"}
    };

    auto result = stevensMapLib::getPairsWhereKeysStartWith(testMap, std::string("style:"));

    EXPECT_EQ(result.size(), 3);
    EXPECT_TRUE(result.count("style:textColor"));
    EXPECT_TRUE(result.count("style:backgroundColor"));
    EXPECT_TRUE(result.count("style:fontSize"));
    EXPECT_FALSE(result.count("sectionName"));
}

TEST(StringOperationTests, GetPairsWhereKeysStartWith_NoMatches)
{
    std::map<std::string, int> testMap = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };

    auto result = stevensMapLib::getPairsWhereKeysStartWith(testMap, std::string("orange"));
    EXPECT_TRUE(result.empty());
}

TEST(StringOperationTests, GetPairsWhereKeysStartWith_AllMatch)
{
    std::map<std::string, int> testMap = {
        {"prefix_one", 1},
        {"prefix_two", 2},
        {"prefix_three", 3}
    };

    auto result = stevensMapLib::getPairsWhereKeysStartWith(testMap, std::string("prefix_"));
    EXPECT_EQ(result.size(), 3);
}

TEST(StringOperationTests, GetPairsWhereKeysStartWith_EmptyMap)
{
    std::map<std::string, int> emptyMap;
    auto result = stevensMapLib::getPairsWhereKeysStartWith(emptyMap, std::string("test"));
    EXPECT_TRUE(result.empty());
}

TEST(StringOperationTests, GetPairsWhereKeysStartWith_EmptyPrefix)
{
    std::map<std::string, int> testMap = {
        {"a", 1},
        {"b", 2}
    };

    auto result = stevensMapLib::getPairsWhereKeysStartWith(testMap, std::string(""));
    EXPECT_EQ(result.size(), 2);  // Empty prefix matches all strings
}

// Test erasePairsWhereKeysStartWith function
TEST(StringOperationTests, ErasePairsWhereKeysStartWith_MatchingKeys)
{
    std::map<std::string, std::string> testMap = {
        {"style:textColor", "red"},
        {"style:backgroundColor", "black"},
        {"sectionName", "header"},
        {"title", "MyTitle"}
    };

    auto result = stevensMapLib::erasePairsWhereKeysStartWith(testMap, std::string("style:"));

    EXPECT_EQ(result.size(), 2);
    EXPECT_FALSE(result.count("style:textColor"));
    EXPECT_FALSE(result.count("style:backgroundColor"));
    EXPECT_TRUE(result.count("sectionName"));
    EXPECT_TRUE(result.count("title"));
}

TEST(StringOperationTests, ErasePairsWhereKeysStartWith_NoMatches)
{
    std::map<std::string, int> testMap = {
        {"apple", 1},
        {"banana", 2}
    };

    auto result = stevensMapLib::erasePairsWhereKeysStartWith(testMap, std::string("orange"));
    EXPECT_EQ(result.size(), 2);
}

TEST(StringOperationTests, ErasePairsWhereKeysStartWith_AllMatch)
{
    std::map<std::string, int> testMap = {
        {"prefix_one", 1},
        {"prefix_two", 2}
    };

    auto result = stevensMapLib::erasePairsWhereKeysStartWith(testMap, std::string("prefix_"));
    EXPECT_TRUE(result.empty());
}

// Test eraseStringFromKeys function
TEST(StringOperationTests, EraseStringFromKeys_RemovePrefix)
{
    std::map<std::string, std::string> testMap = {
        {"style:textColor", "red"},
        {"style:backgroundColor", "black"}
    };

    auto result = stevensMapLib::eraseStringFromKeys(testMap, std::string("style:"));

    EXPECT_EQ(result.size(), 2);
    EXPECT_TRUE(result.count("textColor"));
    EXPECT_TRUE(result.count("backgroundColor"));
    EXPECT_FALSE(result.count("style:textColor"));
    EXPECT_EQ(result.at("textColor"), "red");
    EXPECT_EQ(result.at("backgroundColor"), "black");
}

TEST(StringOperationTests, EraseStringFromKeys_RemoveMiddleString)
{
    std::map<std::string, int> testMap = {
        {"apple_fruit_red", 1},
        {"banana_fruit_yellow", 2}
    };

    auto result = stevensMapLib::eraseStringFromKeys(testMap, std::string("_fruit_"));

    EXPECT_TRUE(result.count("applered"));
    EXPECT_TRUE(result.count("bananayellow"));
}

TEST(StringOperationTests, EraseStringFromKeys_NoMatch)
{
    std::map<std::string, int> testMap = {
        {"apple", 1},
        {"banana", 2}
    };

    auto result = stevensMapLib::eraseStringFromKeys(testMap, std::string("xyz"));

    EXPECT_TRUE(result.count("apple"));
    EXPECT_TRUE(result.count("banana"));
}

TEST(StringOperationTests, EraseStringFromKeys_EmptyString)
{
    std::map<std::string, int> testMap = {
        {"apple", 1},
        {"banana", 2}
    };

    auto result = stevensMapLib::eraseStringFromKeys(testMap, std::string(""));

    EXPECT_TRUE(result.count("apple"));
    EXPECT_TRUE(result.count("banana"));
}

TEST(StringOperationTests, EraseStringFromKeys_EmptyMap)
{
    std::map<std::string, int> emptyMap;
    auto result = stevensMapLib::eraseStringFromKeys(emptyMap, std::string("test"));
    EXPECT_TRUE(result.empty());
}

// Test createUniqueKeyString function
TEST(StringOperationTests, CreateUniqueKeyString_NoConflict)
{
    std::map<std::string, int> testMap = {
        {"apple", 1},
        {"banana", 2}
    };

    auto result = stevensMapLib::createUniqueKeyString(testMap, std::string("cherry"));
    EXPECT_EQ(result, "cherry");
}

TEST(StringOperationTests, CreateUniqueKeyString_WithConflict)
{
    std::map<std::string, int> testMap = {
        {"item", 1},
        {"item0", 2}
    };

    auto result = stevensMapLib::createUniqueKeyString(testMap, std::string("item"));
    EXPECT_EQ(result, "item1");
}

TEST(StringOperationTests, CreateUniqueKeyString_MultipleConflicts)
{
    std::map<std::string, int> testMap = {
        {"key", 1},
        {"key0", 2},
        {"key1", 3},
        {"key2", 4}
    };

    auto result = stevensMapLib::createUniqueKeyString(testMap, std::string("key"));
    EXPECT_EQ(result, "key3");
}

TEST(StringOperationTests, CreateUniqueKeyString_EmptyString)
{
    std::map<std::string, int> testMap = {
        {"", 1},
        {"0", 2}
    };

    auto result = stevensMapLib::createUniqueKeyString(testMap, std::string(""));
    EXPECT_EQ(result, "1");
}

TEST(StringOperationTests, CreateUniqueKeyString_EmptyMap)
{
    std::map<std::string, int> emptyMap;
    auto result = stevensMapLib::createUniqueKeyString(emptyMap, std::string("test"));
    EXPECT_EQ(result, "test");
}
