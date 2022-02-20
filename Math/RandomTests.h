#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "Random.h"

#include "../Utils/Utils.h"

#include <iterator>
#include <map>
#include <string>
#include <vector>

#include <gtest/gtest.h>

using namespace Math_NS;
using namespace Utils_NS;
using namespace std;

namespace {
    void checkDeviations(const map<int, int>& cnt, int maxDeviation) {
        if (cnt.empty())
            return;

        auto minValue = cnt.begin()->second;
        auto maxValue = minValue;

        for (const auto& [_, count]: cnt) {
            std::ignore = _;
            maxValue = max(maxValue, count);
            minValue = min(minValue, count);
        }

        EXPECT_LT(abs(maxValue - minValue), maxDeviation);
    }

    #define VALUES 2,10,100
    #define TESTING_VALUES ::testing::Values(VALUES)
    #define REPEATS 10'000'000
    const map<pair<pair<pair<int, int>, string>, string>, int> ExpectedDeviations = {
        {{{{REPEATS, 2  }, "random" }, ""     },    11000},
        {{{{REPEATS, 10 }, "random" }, ""     },     6100},
        {{{{REPEATS, 100}, "random" }, ""     },     2500},
        {{{{REPEATS, 2  }, "randomC"}, ""     },    11000},
        {{{{REPEATS, 2  }, "randomC"}, "float"},  5010000},
        {{{{REPEATS, 10 }, "randomC"}, ""     },     6100},
        {{{{REPEATS, 10 }, "randomC"}, "float"},  1002000},
        {{{{REPEATS, 100}, "randomC"}, ""     },     2500},
        {{{{REPEATS, 100}, "randomC"}, "float"},   102000}
    };

    int getExpectedDeviations(int repeats, int bins, const string& func, const string& type) {
        auto it = ExpectedDeviations.find({{{repeats, bins}, func}, type});
        if (it != ExpectedDeviations.end())
            return it->second;

        it = ExpectedDeviations.find({{{repeats, bins}, func}, ""});
        if (it != ExpectedDeviations.end())
            return it->second;

        return 0;
    }

    const vector<int> Bins = {VALUES};
}

#define TST_EXPECTED_VALUES(TYPE,RAND)             \
TEST(RandomTest, TYPE##_##ExpectedValues_##RAND) { \
    for (int i = 0; i < REPEATS; ++i) {            \
        auto rnd = RAND<TYPE>(10, 100);            \
        ASSERT_GE(rnd, 10 );                       \
        ASSERT_LT(rnd, 100);                       \
    }                                              \
}

#define TST_GOOD_UNIFORMITY(TYPE,RUNS,RAND)                                                   \
class TYPE##_GoodUniformity_x##RUNS##_##RAND : public ::testing::TestWithParam<int> {};       \
TEST_P(TYPE##_GoodUniformity_x##RUNS##_##RAND, Test) {                                        \
    for (int run = 0; run < RUNS; ++run) {                                                    \
        map<int,int> cnt;                                                                     \
        for (int i = 0; i < REPEATS; ++i)                                                     \
            ++cnt[static_cast<int>(RAND<TYPE>(0, GetParam()))];                               \
        checkDeviations(cnt, getExpectedDeviations(REPEATS, GetParam(), #RAND, #TYPE));       \
    }                                                                                         \
}                                                                                             \
INSTANTIATE_TEST_SUITE_P(RandomTest, TYPE##_GoodUniformity_x##RUNS##_##RAND, TESTING_VALUES);

#define TST_STABILITY_BINS_INCREASE(TYPE,RUNS,RAND)                                        \
TEST(RandomTests, TYPE##_StabilityBinsIncrease_##RAND) {                                   \
    for (int bin = 0; bin < isize(Bins); ++bin) {                                          \
        for (int run = 0; run < RUNS; ++run) {                                             \
            map<int,int> cnt;                                                              \
            for (int i = 0; i < REPEATS; ++i)                                              \
                ++cnt[static_cast<int>(RAND<TYPE>(0, Bins[bin]))];                         \
            EXPECT_GE(cnt.begin()->first, 0);                                              \
            if (#RAND == "randomC" && #TYPE == "float")                                    \
                EXPECT_LE(prev(cnt.end())->first, Bins[bin]);                              \
            else                                                                           \
                EXPECT_LT(prev(cnt.end())->first, Bins[bin]);                              \
            checkDeviations(cnt, getExpectedDeviations(REPEATS, Bins[bin], #RAND, #TYPE)); \
        }                                                                                  \
    }                                                                                      \
}

#define TST_STABILITY_BINS_DECREASE(TYPE,RUNS,RAND)                                        \
TEST(RandomTests, TYPE##_StabilityBinsDecrease_##RAND) {                                   \
    for (int bin = isize(Bins)-1; bin >= 0; --bin) {                                       \
        for (int run = 0; run < RUNS; ++run) {                                             \
            map<int,int> cnt;                                                              \
            for (int i = 0; i < REPEATS; ++i)                                              \
                ++cnt[static_cast<int>(RAND<TYPE>(0, Bins[bin]))];                         \
            EXPECT_GE(cnt.begin()->first, 0);                                              \
            if (#RAND == "randomC" && #TYPE == "float")                                    \
                EXPECT_LE(prev(cnt.end())->first, Bins[bin]);                              \
            else                                                                           \
                EXPECT_LT(prev(cnt.end())->first, Bins[bin]);                              \
            checkDeviations(cnt, getExpectedDeviations(REPEATS, Bins[bin], #RAND, #TYPE)); \
        }                                                                                  \
    }                                                                                      \
}

#define TST_FULL_TEST(TYPE,RUNS,RAND)       \
TST_EXPECTED_VALUES(TYPE,RAND)              \
TST_GOOD_UNIFORMITY(TYPE,RUNS,RAND)         \
TST_STABILITY_BINS_INCREASE(TYPE,RUNS,RAND) \
TST_STABILITY_BINS_DECREASE(TYPE,RUNS,RAND)

#define FULL_TEST(TYPE,RUNS)     \
TST_FULL_TEST(TYPE,RUNS,random)  \
TST_FULL_TEST(TYPE,RUNS,randomC)

#define TEST_RUNS 10
FULL_TEST(int   , TEST_RUNS);
FULL_TEST(long  , TEST_RUNS);
FULL_TEST(float , TEST_RUNS);
FULL_TEST(double, TEST_RUNS);
