#pragma once

#include "../AD/RT.h"
#include "../AD/Quadrature.h"

#include <gtest/gtest.h>

template <typename T, size_t M>
class testDataRT {
public:
    testDataRT(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom) {
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

    T getRs() const noexcept {
        return rs;
    }

    T getTs() const noexcept {
        return ts;
    }

    T getTc() const noexcept {
        return tc;
    }

protected:
    T myA, myTau, myG, myN_slab, myN_slide_top, myN_slide_bottom, ts, rs, tc;
    std::array<T, M> v, w;
};

TEST(AD_FinalRT, RTtest1) {
    testDataRT<double, 32> test(0.9, 1.0, 0.9, 1.4, 1.5, 1.5);
    EXPECT_NEAR(test.getRs(), 0.08531, 1e-4);
    EXPECT_NEAR(test.getTs(), 0.77350, 1e-4);
    EXPECT_NEAR(test.getTc(), 0.338341, 1e-4);
}

TEST(AD_FinalRT, RTtest2) {
    testDataRT<float, 16> test(0.9, 2.0, 0.99, 1.5, 1.5, 1.5);
    EXPECT_NEAR(test.getRs(), 0.06548, 1e-4);
    EXPECT_NEAR(test.getTs(), 0.74409, 1e-4);
    EXPECT_NEAR(test.getTc(), 0.124729, 1e-4);
}

TEST(AD_FinalRT, RTtestG0) {
    testDataRT<double, 4> test(0.95, 5.0, 0.0, 1.4, 1.4, 1.4);
    EXPECT_NEAR(test.getRs(), 0.38911, 1e-4);
    EXPECT_NEAR(test.getTs(), 0.11869, 1e-4);
    EXPECT_NEAR(test.getTc(), 0.006369, 1e-4);
}

TEST(AD_FinalRT, RTtestA0) {
    testDataRT<float, 8> test(0.95, 5.0, 0.0, 1.5, 1.6, 1.6);
    EXPECT_NEAR(test.getRs(), 0.07204, 1e-4);
    EXPECT_NEAR(test.getTs(), 0.54314, 1e-4);
    EXPECT_NEAR(test.getTc(), 0.543166, 1e-4);
    EXPECT_NEAR(test.getTc(), test.getTs(), 1e-3);
}

TEST(AD_FinalRT, RTtestA0G0) {
    testDataRT<double, 32> test(0.0, 1.0, 0.0, 1.3, 1.4, 1.4);
    EXPECT_NEAR(test.getRs(), 0.03278, 1e-4);
    EXPECT_NEAR(test.getTs(), 0.34684, 1e-4);
    EXPECT_NEAR(test.getTc(), 0.346838, 1e-4);
    EXPECT_NEAR(test.getTc(), test.getTs(), 1e-3);
}
