#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "Random.h"

#include "../Utils/Utils.h"

#include <iterator>
#include <map>
#include <vector>

#include <gtest/gtest.h>

using namespace Math_NS;
using namespace Utils_NS;
using namespace std;

namespace {
    void checkDeviations(const map<int, int>& cnt, int maxDeviation) {
        if (cnt.empty())
            return;

        auto minValue = (*cnt.begin()).second;
        auto maxValue = (*cnt.begin()).second;

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
    const map<pair<int, int>, int> ExpectedDeviations = {
        {{REPEATS, 2  }, 7900},
        {{REPEATS, 10 }, 5300},
        {{REPEATS, 100}, 2500}
    };
    const vector<int> Bins = {VALUES};
}

#define TST_EXPECTED_VALUES(TYPE)           \
TEST(RandomTest, TYPE##_##ExpectedValues) { \
    for (int i = 0; i < REPEATS; ++i) {     \
        auto rnd = random<TYPE>(10, 100);   \
        ASSERT_GE(rnd, 10 );                \
        ASSERT_LT(rnd, 100);                \
    }                                       \
}

#define TST_GOOD_UNIFORMITY(TYPE,RUNS)                                              \
class TYPE##_GoodUniformity_x##RUNS : public ::testing::TestWithParam<int> {};      \
TEST_P(TYPE##_GoodUniformity_x##RUNS, Test) {                                       \
    for (int run = 0; run < RUNS; ++run) {                                          \
        map<int,int> cnt;                                                           \
        for (int i = 0; i < REPEATS; ++i)                                           \
            ++cnt[static_cast<int>(random<TYPE>(0, GetParam()))];                   \
        checkDeviations(cnt, ExpectedDeviations.at({REPEATS, GetParam()}));         \
    }                                                                               \
}                                                                                   \
INSTANTIATE_TEST_CASE_P(RandomTest, TYPE##_GoodUniformity_x##RUNS, TESTING_VALUES);

#define TST_STABILITY_BINS_INCREASE(TYPE,RUNS)                                 \
TEST(RandomTests, TYPE##_StabilityBinsIncrease) {                              \
    for (int bin = 0; bin < isize(Bins); ++bin) {                              \
        for (int run = 0; run < RUNS; ++run) {                                 \
            map<int,int> cnt;                                                  \
            for (int i = 0; i < REPEATS; ++i)                                  \
                ++cnt[static_cast<int>(random<TYPE>(0, Bins[bin]))];           \
            EXPECT_GE((*cnt.begin()).first    , 0        );                    \
            EXPECT_LT((*prev(cnt.end())).first, Bins[bin]);                    \
            checkDeviations(cnt, ExpectedDeviations.at({REPEATS, Bins[bin]})); \
        }                                                                      \
    }                                                                          \
}

#define TST_STABILITY_BINS_DECREASE(TYPE,RUNS)                                 \
TEST(RandomTests, TYPE##_StabilityBinsDecrease) {                              \
    for (int bin = isize(Bins)-1; bin >= 0; --bin) {                           \
        for (int run = 0; run < RUNS; ++run) {                                 \
            map<int,int> cnt;                                                  \
            for (int i = 0; i < REPEATS; ++i)                                  \
                ++cnt[static_cast<int>(random<TYPE>(0, Bins[bin]))];           \
            EXPECT_GE((*cnt.begin()).first    , 0        );                    \
            EXPECT_LT((*prev(cnt.end())).first, Bins[bin]);                    \
            checkDeviations(cnt, ExpectedDeviations.at({REPEATS, Bins[bin]})); \
        }                                                                      \
    }                                                                          \
}

#define FULL_TEST(TYPE,RUNS)           \
TST_EXPECTED_VALUES(TYPE)              \
TST_GOOD_UNIFORMITY(TYPE,RUNS)         \
TST_STABILITY_BINS_INCREASE(TYPE,RUNS) \
TST_STABILITY_BINS_DECREASE(TYPE,RUNS)

#define TEST_RUNS 10
FULL_TEST(int,TEST_RUNS);
FULL_TEST(long,TEST_RUNS);
FULL_TEST(float,TEST_RUNS);
FULL_TEST(double,TEST_RUNS);
