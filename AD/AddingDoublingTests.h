#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "AddingDoubling.h"
#include "RT.h"
#include "Quadrature.h"
#include "../MC/Sample.h"

#include "../Tests/BenchmarkHelper.h"

#include <gtest/gtest.h>

using namespace AddingDoubling_NS;
using namespace std;

template < typename T, size_t M >
class TestDataRT {
public:
    TestDataRT(T a, T tau, T g, T nSlab, T nSlideTop, T nSlideBottom) EXCEPT_INPUT_PARAMS
               : A(a)
               , Tau(tau)
               , G(g)
               , NSlab(nSlab)
               , NSlideTop(nSlideTop)
               , NSlideBottom(nSlideBottom) {
        setValues();
        calc();
    }

    void setValues() {
        Quadrature<T,M> quadrature(NSlab);
        v = quadrature.getV();
        w = quadrature.getW();
    }

    void calc() {
        const auto layer = Medium<T>::fromAlbedo(NSlab, A, Tau, 1.0, G);
        const auto glassTop = Medium<T>::fromCoeffs(NSlideTop, 0.0, 0.0, 1.0, 0.0);
        const auto glassBottom = Medium<T>::fromCoeffs(NSlideBottom, 0.0, 0.0, 1.0, 0.0);
        vector<Medium<T>> samples;
        if (NSlideTop == NSlab)
            samples = {layer};
        else
            samples = {glassTop, layer, glassBottom};
        RTs<T,M>(Sample<T>(samples), v, w, rs, ts);
        tc = Tc<T,M>(Tau, NSlab, NSlideTop, NSlideBottom);
    }

    T getRs() const noexcept { return rs; }
    T getTs() const noexcept { return ts; }
    T getTc() const noexcept { return tc; }

protected:
    T A, Tau, G, NSlab, NSlideTop, NSlideBottom, ts, rs, tc;
    array<T,M> v, w;
};

void RawTest1() {
    constexpr double TOLERANCE = 1e-4;

    TestDataRT<double,32> test(0.9, 1.0, 0.9, 1.4, 1.5, 1.5);
    EXPECT_NEAR(test.getRs(), 0.08531 , TOLERANCE);
    EXPECT_NEAR(test.getTs(), 0.77350 , TOLERANCE);
    EXPECT_NEAR(test.getTc(), 0.338341, TOLERANCE);
}

BENCHMARK_TEST(AddingDoubling, Test1, RawTest1, 100, 3600)

void RawTest2() {
    constexpr double TOLERANCE = 1e-4;

    TestDataRT<float,16> test(0.9, 2.0, 0.99, 1.5, 1.5, 1.5);
    EXPECT_NEAR(test.getRs(), 0.06548 , TOLERANCE);
    EXPECT_NEAR(test.getTs(), 0.74409 , TOLERANCE);
    EXPECT_NEAR(test.getTc(), 0.124729, TOLERANCE);
}

BENCHMARK_TEST(AddingDoubling, Test2, RawTest2, 100, 700)

void RawG0() {
    constexpr double TOLERANCE = 1e-4;

    TestDataRT<double,4> test(0.95, 5.0, 0.0, 1.4, 1.4, 1.4);
    EXPECT_NEAR(test.getRs(), 0.38911 , TOLERANCE);
    EXPECT_NEAR(test.getTs(), 0.11869 , TOLERANCE);
    EXPECT_NEAR(test.getTc(), 0.006369, TOLERANCE);
}

BENCHMARK_TEST(AddingDoubling, G0, RawG0, 10000, 1300)

void RawA0() {
    constexpr double TOLERANCE   = 1e-4;
    constexpr double TOLERANCE_T = 1e-3;

    TestDataRT<float,8> test(0.0, 0.5, 0.9, 1.5, 1.6, 1.6);
    EXPECT_NEAR(test.getRs(), 0.07204     , TOLERANCE  );
    EXPECT_NEAR(test.getTs(), 0.54314     , TOLERANCE  );
    EXPECT_NEAR(test.getTc(), 0.543166    , TOLERANCE  );
    EXPECT_NEAR(test.getTc(), test.getTs(), TOLERANCE_T);
}

BENCHMARK_TEST(AddingDoubling, A0, RawA0, 1000, 1000)

void RawA0G0() {
    constexpr double TOLERANCE   = 1e-4;
    constexpr double TOLERANCE_T = 1e-3;

    TestDataRT<double,32> test(0.0, 1.0, 0.0, 1.3, 1.4, 1.4);
    EXPECT_NEAR(test.getRs(), 0.03278     , TOLERANCE  );
    EXPECT_NEAR(test.getTs(), 0.34684     , TOLERANCE  );
    EXPECT_NEAR(test.getTc(), 0.346838    , TOLERANCE  );
    EXPECT_NEAR(test.getTc(), test.getTs(), TOLERANCE_T);
}

BENCHMARK_TEST(AddingDoubling, A0_G0, RawA0G0, 100, 800)
