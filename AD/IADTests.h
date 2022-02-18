#pragma once

#include "NelderMead.h"
#include "IAD.h"

#include <gtest/gtest.h>

/// TODO: make some statistics as in MC tests
/// TODO: extract tolerance

TEST(IAD, Test1) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 4;
    constexpr int N = 2; // minimize 2 parameters
    constexpr bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    constexpr T n_slab = 1.4; // refraction index of sample
    constexpr T n_slide_top = 1.5; // refraction index of slide
    constexpr T n_slide_bottom = 1.5;
    constexpr T rsmeas = 0.08624;
    constexpr T tsmeas = 0.76446;
    constexpr T tcmeas = 0.338341;
    T aOut, tauOut, gOut;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0.9, TOLERANCE);
    EXPECT_NEAR(tauOut, 1  , TOLERANCE);
    EXPECT_NEAR(gOut  , 0.9, TOLERANCE);
}

TEST(IAD, Test2) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 32;
    constexpr int N = 2; // minimize 2 parameters
    constexpr bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    constexpr T n_slab = 1.4; // refraction index of sample
    constexpr T n_slide_top = 1.5; // refraction index of slide
    constexpr T n_slide_bottom = 1.5;
    constexpr T rsmeas = 0.08531;
    constexpr T tsmeas = 0.77350;
    constexpr T tcmeas = 0.338341;
    T aOut, tauOut, gOut;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0.9, TOLERANCE);
    EXPECT_NEAR(tauOut, 1  , TOLERANCE);
    EXPECT_NEAR(gOut  , 0.9, TOLERANCE);
}

TEST(IAD, Test3) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 16;
    constexpr int N = 2; // minimize 2 parameters
    constexpr bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    constexpr T n_slab = 1.5; // refraction index of sample
    constexpr T n_slide_top = 1.5; // refraction index of slide
    constexpr T n_slide_bottom = 1.5;
    constexpr T rsmeas = 0.06548;
    constexpr T tsmeas = 0.74409;
    constexpr T tcmeas = 0.124729;
    T aOut, tauOut, gOut;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0.9 , TOLERANCE);
    EXPECT_NEAR(tauOut, 2   , TOLERANCE);
    EXPECT_NEAR(gOut  , 0.99, TOLERANCE);
}

TEST(IAD, G0) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 4;
    constexpr int N = 2; // minimize 2 parameters
    constexpr bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    constexpr T n_slab = 1.4; // refraction index of sample
    constexpr T n_slide_top = 1.4; // refraction index of slide
    constexpr T n_slide_bottom = 1.4;
    constexpr T rsmeas = 0.38911;
    constexpr T tsmeas = 0.11869;
    constexpr T tcmeas = 0.006369;
    T aOut, tauOut, gOut;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0.95, TOLERANCE);
    EXPECT_NEAR(tauOut, 5   , TOLERANCE);
    EXPECT_NEAR(gOut  , 0   , TOLERANCE);
}

TEST(IAD, A0_G0_Tau_05) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 8;
    constexpr int N = 2; // minimize 2 parameters
    constexpr bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    constexpr T n_slab = 1.5; // refraction index of sample
    constexpr T n_slide_top = 1.6; // refraction index of slide
    constexpr T n_slide_bottom = 1.6;
    constexpr T rsmeas = 0.07204;
    constexpr T tsmeas = 0.54314;
    constexpr T tcmeas = 0.543166;
    T aOut, tauOut, gOut;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0  ,      TOLERANCE);
    EXPECT_NEAR(tauOut, 0.5,      TOLERANCE);
    // When a = 0, no value of g is correct
    // EXPECT_NEAR(gOut  , 0.9  , 10 * TOLERANCE);
}

TEST(IAD, A0_G0_Tau1) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 32;
    constexpr int N = 2; // minimize 2 parameters
    constexpr bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    constexpr T n_slab = 1.3; // refraction index of sample
    constexpr T n_slide_top = 1.4; // refraction index of slide
    constexpr T n_slide_bottom = 1.4;
    constexpr T rsmeas = 0.03278;
    constexpr T tsmeas = 0.34684;
    constexpr T tcmeas = 0.346838;
    T aOut, tauOut, gOut;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0,     TOLERANCE);
    EXPECT_NEAR(tauOut, 1,     TOLERANCE);
    // When a = 0, no value of g is correct
    // EXPECT_NEAR(gOut  , 0, 5 * TOLERANCE);
}
