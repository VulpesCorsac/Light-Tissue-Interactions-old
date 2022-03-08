#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "AddingDoubling.h"
#include "RT.h"
#include "Quadrature.h"

#include "../Tests/BenchmarkHelper.h"

#include <gtest/gtest.h>

using namespace AddingDoubling_NS;
using namespace std;

template < typename T, size_t M >
class TestDataRT {
public:
    TestDataRT(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom) {
        setValues(a, tau, g, n_slab, n_slide_top, n_slide_bottom);
        calc();
    }

    void setValues(const T& a, const T& tau, const T& g, const T& n_slab, const T& n_slide_top, const T& n_slide_bottom) {
        myA = a;
        myTau = tau;
        myG = g;
        myN_slab = n_slab;
        myN_slide_top = n_slide_top;
        myN_slide_bottom = n_slide_bottom;

        Quadrature<T,M> quadrature(n_slab);
        v = quadrature.getV();
        w = quadrature.getW();
    }

    void calc() {
        RTs<T,M>(myA, myTau, myG, myN_slab, myN_slide_top, myN_slide_bottom, v, w, rs, ts);
        tc = Tc<T,M>(myTau, myN_slab, myN_slide_top, myN_slide_bottom);
    }

    T getRs() const noexcept { return rs; }
    T getTs() const noexcept { return ts; }
    T getTc() const noexcept { return tc; }

protected:
    T myA, myTau, myG, myN_slab, myN_slide_top, myN_slide_bottom, ts, rs, tc;
    array<T,M> v, w;
};

void RawTest1() {
    constexpr double TOLERANCE = 1e-4;

    TestDataRT<double,32> test(0.9, 1.0, 0.9, 1.4, 1.5, 1.5);
    EXPECT_NEAR(test.getRs(), 0.08531 , TOLERANCE);
    EXPECT_NEAR(test.getTs(), 0.77350 , TOLERANCE);
    EXPECT_NEAR(test.getTc(), 0.338341, TOLERANCE);
}

BENCHMARK_TEST(AddingDoubling, Test1, RawTest1, 100, 3500)

void RawTest2() {
    constexpr double TOLERANCE = 1e-4;

    TestDataRT<float,16> test(0.9, 2.0, 0.99, 1.5, 1.5, 1.5);
    EXPECT_NEAR(test.getRs(), 0.06548 , TOLERANCE);
    EXPECT_NEAR(test.getTs(), 0.74409 , TOLERANCE);
    EXPECT_NEAR(test.getTc(), 0.124729, TOLERANCE);
}

BENCHMARK_TEST(AddingDoubling, Test2, RawTest2, 100, 500)

void RawG0() {
    constexpr double TOLERANCE = 1e-4;

    TestDataRT<double,4> test(0.95, 5.0, 0.0, 1.4, 1.4, 1.4);
    EXPECT_NEAR(test.getRs(), 0.38911 , TOLERANCE);
    EXPECT_NEAR(test.getTs(), 0.11869 , TOLERANCE);
    EXPECT_NEAR(test.getTc(), 0.006369, TOLERANCE);
}

BENCHMARK_TEST(AddingDoubling, G0, RawG0, 10000, 800)

void RawA0() {
    constexpr double TOLERANCE   = 1e-4;
    constexpr double TOLERANCE_T = 1e-3;

    TestDataRT<float,8> test(0.0, 0.5, 0.9, 1.5, 1.6, 1.6);
    EXPECT_NEAR(test.getRs(), 0.07204     , TOLERANCE  );
    EXPECT_NEAR(test.getTs(), 0.54314     , TOLERANCE  );
    EXPECT_NEAR(test.getTc(), 0.543166    , TOLERANCE  );
    EXPECT_NEAR(test.getTc(), test.getTs(), TOLERANCE_T);
}

BENCHMARK_TEST(AddingDoubling, A0, RawA0, 1000, 700)

void RawA0G0() {
    constexpr double TOLERANCE   = 1e-4;
    constexpr double TOLERANCE_T = 1e-3;

    TestDataRT<double,32> test(0.0, 1.0, 0.0, 1.3, 1.4, 1.4);
    EXPECT_NEAR(test.getRs(), 0.03278     , TOLERANCE  );
    EXPECT_NEAR(test.getTs(), 0.34684     , TOLERANCE  );
    EXPECT_NEAR(test.getTc(), 0.346838    , TOLERANCE  );
    EXPECT_NEAR(test.getTc(), test.getTs(), TOLERANCE_T);
}

BENCHMARK_TEST(AddingDoubling, A0_G0, RawA0G0, 100, 450)
