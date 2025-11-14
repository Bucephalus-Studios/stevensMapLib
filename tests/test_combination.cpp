#include <gtest/gtest.h>
#include "stevensMapLib.hpp"
#include <map>
#include <unordered_map>

// Test addMaps function
TEST(CombinationTests, AddMaps_ValuesOnly_SharedKeys)
{
    std::map<std::string, int> mapA = {{"a", 5}, {"b", 10}, {"c", 15}};
    std::map<std::string, int> mapB = {{"a", 3}, {"b", 7}, {"d", 20}};

    auto result = stevensMapLib::addMaps(mapA, mapB, "values", false);

    EXPECT_EQ(result.at("a"), 8);   // 5 + 3
    EXPECT_EQ(result.at("b"), 17);  // 10 + 7
    EXPECT_EQ(result.at("c"), 15);  // not in B
    EXPECT_EQ(result.at("d"), 20);  // not in A
}

TEST(CombinationTests, AddMaps_ValuesOnly_OmitNonShared)
{
    std::map<std::string, int> mapA = {{"a", 5}, {"b", 10}, {"c", 15}};
    std::map<std::string, int> mapB = {{"a", 3}, {"b", 7}, {"d", 20}};

    auto result = stevensMapLib::addMaps(mapA, mapB, "values", true);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result.at("a"), 8);   // 5 + 3
    EXPECT_EQ(result.at("b"), 17);  // 10 + 7
    EXPECT_FALSE(result.count("c"));
    EXPECT_FALSE(result.count("d"));
}

TEST(CombinationTests, AddMaps_KeysAndValues)
{
    std::map<std::string, int> mapA = {{"a", 5}, {"b", 10}};
    std::map<std::string, int> mapB = {{"a", 3}, {"c", 7}};

    auto result = stevensMapLib::addMaps(mapA, mapB, "keys and values", false);

    EXPECT_EQ(result.at("aa"), 8);  // key "a" + "a", value 5 + 3
    EXPECT_EQ(result.at("b"), 10);  // not in B
    EXPECT_EQ(result.at("c"), 7);   // not in A
}

TEST(CombinationTests, AddMaps_EmptyMaps)
{
    std::map<int, int> emptyA, emptyB;
    auto result = stevensMapLib::addMaps(emptyA, emptyB, "values", false);
    EXPECT_TRUE(result.empty());
}

TEST(CombinationTests, AddMaps_UnorderedMap)
{
    std::unordered_map<int, double> mapA = {{1, 1.5}, {2, 2.5}};
    std::unordered_map<int, double> mapB = {{1, 0.5}, {3, 3.5}};

    auto result = stevensMapLib::addMaps(mapA, mapB, "values", false);

    EXPECT_DOUBLE_EQ(result.at(1), 2.0);
    EXPECT_DOUBLE_EQ(result.at(2), 2.5);
    EXPECT_DOUBLE_EQ(result.at(3), 3.5);
}

// Test combineMaps function
TEST(CombinationTests, CombineMaps_NoOverwrite)
{
    std::map<std::string, int> source = {{"a", 1}, {"b", 2}, {"c", 3}};
    std::map<std::string, int> dest = {{"b", 20}, {"d", 40}};

    auto result = stevensMapLib::combineMaps(source, dest, false);

    EXPECT_EQ(result.at("a"), 1);
    EXPECT_EQ(result.at("b"), 20);  // dest value preserved
    EXPECT_EQ(result.at("c"), 3);
    EXPECT_EQ(result.at("d"), 40);
}

TEST(CombinationTests, CombineMaps_WithOverwrite)
{
    std::map<std::string, int> source = {{"a", 1}, {"b", 2}, {"c", 3}};
    std::map<std::string, int> dest = {{"b", 20}, {"d", 40}};

    auto result = stevensMapLib::combineMaps(source, dest, true);

    EXPECT_EQ(result.at("a"), 1);
    EXPECT_EQ(result.at("b"), 2);   // source value used
    EXPECT_EQ(result.at("c"), 3);
    EXPECT_EQ(result.at("d"), 40);
}

TEST(CombinationTests, CombineMaps_EmptySource)
{
    std::map<int, int> source;
    std::map<int, int> dest = {{1, 10}, {2, 20}};

    auto result = stevensMapLib::combineMaps(source, dest, false);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result.at(1), 10);
}

TEST(CombinationTests, CombineMaps_EmptyDest)
{
    std::map<int, int> source = {{1, 10}, {2, 20}};
    std::map<int, int> dest;

    auto result = stevensMapLib::combineMaps(source, dest, true);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result.at(1), 10);
}
