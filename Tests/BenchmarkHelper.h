#pragma once

#include "../Utils/Time.h"

namespace Tests_NS {
    /// Helper class for benchmarks tests
    class BenchmarkHelper {
    public:
        /// start timer
        void start() noexcept;
        /// finish timer
        void finish() noexcept;
        /// Check if milliseconds elapsed from timer start point to time point is less or equal to duration
        /// \param[in] duration duration to compare elapsed with
        /// \return If milliseconds elapsed from timer start point to time point is less or equal to duration
        bool elapsed_le(double duration) noexcept;

    protected:
        Utils_NS::Time time; ///< class to evaluate timer
    };
}

#define BENCHMARK_TEST(TEST_CASE_NAME,TEST_NAME,TEST_FUNC_NAME,CNT,DURATION_MS)         \
    TEST(TEST_CASE_NAME, TEST_NAME##_SingleRun) {                                       \
        TEST_FUNC_NAME();                                                               \
    }                                                                                   \
                                                                                        \
    TEST(TEST_CASE_NAME, TEST_NAME##_Benchmark##_x##CNT##_Expected_##DURATION_MS##ms) { \
        Tests_NS::BenchmarkHelper timer;                                                \
        timer.start();                                                                  \
                                                                                        \
        for (int cnt = 0; cnt < CNT; ++cnt)                                             \
            TEST_FUNC_NAME();                                                           \
                                                                                        \
        timer.finish();                                                                 \
        EXPECT_TRUE(timer.elapsed_le(DURATION_MS));                                     \
    }                                                                                   \

/******************
 * IMPLEMENTATION *
 ******************/

void Tests_NS::BenchmarkHelper::start() noexcept {
    time.start();
}

void Tests_NS::BenchmarkHelper::finish() noexcept {
    time.finish();
}

bool Tests_NS::BenchmarkHelper::elapsed_le(double duration) noexcept {
    return time.msecElapsedDuration() < duration;
}
