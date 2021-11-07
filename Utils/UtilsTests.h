#pragma once

#include "Utils.h"

#include <gtest/gtest.h>

#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <deque>

using namespace Utils_NS;

TEST(UtilsTests, Isize_Vector) {
    std::vector<int> data;
    EXPECT_EQ(isize(data), 0);
    data.push_back(1);
    EXPECT_EQ(isize(data), 1);
    data.push_back(2);
    EXPECT_EQ(isize(data), 2);
    data.clear();
    EXPECT_EQ(isize(data), 0);
}

TEST(UtilsTests, Isize_Deque) {
    std::deque<int> data;
    EXPECT_EQ(isize(data), 0);
    data.push_back(1);
    EXPECT_EQ(isize(data), 1);
    data.push_back(2);
    EXPECT_EQ(isize(data), 2);
    data.clear();
    EXPECT_EQ(isize(data), 0);
}

TEST(UtilsTests, Isize_Set) {
    std::set<int> data;
    EXPECT_EQ(isize(data), 0);
    data.insert(1);
    EXPECT_EQ(isize(data), 1);
    data.insert(2);
    EXPECT_EQ(isize(data), 2);
    data.clear();
    EXPECT_EQ(isize(data), 0);
}

TEST(UtilsTests, Isize_UnorderedSet) {
    std::unordered_set<int> data;
    EXPECT_EQ(isize(data), 0);
    data.insert(1);
    EXPECT_EQ(isize(data), 1);
    data.insert(2);
    EXPECT_EQ(isize(data), 2);
    data.clear();
    EXPECT_EQ(isize(data), 0);
}

TEST(UtilsTests, Isize_Map) {
    std::map<int, int> data;
    EXPECT_EQ(isize(data), 0);
    data[1] = 1;
    EXPECT_EQ(isize(data), 1);
    data[2] = 2;
    EXPECT_EQ(isize(data), 2);
    data.clear();
    EXPECT_EQ(isize(data), 0);
}

TEST(UtilsTests, Isize_UnorderedMap) {
    std::map<int, int> data;
    EXPECT_EQ(isize(data), 0);
    data[1] = 1;
    EXPECT_EQ(isize(data), 1);
    data[2] = 2;
    EXPECT_EQ(isize(data), 2);
    data.clear();
    EXPECT_EQ(isize(data), 0);
}

TEST(UtilsTests, Contains_Set) {
    std::set<int> data;
    EXPECT_FALSE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
    data.insert(1);
    EXPECT_TRUE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
    data.insert(2);
    EXPECT_TRUE(contains(data, 1));
    EXPECT_TRUE(contains(data, 2));
    data.clear();
    EXPECT_FALSE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
}

TEST(UtilsTests, Contains_UnorderedSet) {
    std::unordered_set<int> data;
    EXPECT_FALSE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
    data.insert(1);
    EXPECT_TRUE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
    data.insert(2);
    EXPECT_TRUE(contains(data, 1));
    EXPECT_TRUE(contains(data, 2));
    data.clear();
    EXPECT_FALSE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
}

TEST(UtilsTests, Contains_Map) {
    std::map<int, int> data;
    EXPECT_FALSE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
    data[1] = 5;
    EXPECT_TRUE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
    data[2] = 6;
    EXPECT_TRUE(contains(data, 1));
    EXPECT_TRUE(contains(data, 2));
    data.clear();
    EXPECT_FALSE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
}

TEST(UtilsTests, Contains_UnorderedMap) {
    std::unordered_map<int, int> data;
    EXPECT_FALSE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
    data[1] = 5;
    EXPECT_TRUE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
    data[2] = 6;
    EXPECT_TRUE(contains(data, 1));
    EXPECT_TRUE(contains(data, 2));
    data.clear();
    EXPECT_FALSE(contains(data, 1));
    EXPECT_FALSE(contains(data, 2));
}
