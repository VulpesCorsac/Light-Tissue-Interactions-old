#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "AddingDoublingNelderMead.h"
#include "InverseAddingDoubling.h"

#include <gtest/gtest.h>

/// TODO: make some statistics as in MC tests
/// TODO: extract tolerance

using namespace AddingDoubling_NS;
using namespace Inverse_NS;

TEST(InverseAddingDoubling, Test1) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 4;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.08624;
    constexpr T tsmeas = 0.76446;
    constexpr T tcmeas = 0.338341;

    T aOut, tauOut, gOut;
    T fixedParamVal = fixParam<T,M,N,fix>(0.0, nSlab, nSlideTop, nSlideBottom, tcmeas);
    T astart, gstart, tstart;
    Func<T,M,N,fix> toMinimize(fixedParamVal, nSlab, nSlideTop, nSlideBottom, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, astart, tstart, gstart);
    IAD<T,M,N,fix>(toMinimize, rsmeas, tsmeas, tcmeas, nSlab, nSlideTop, nSlideBottom, fixedParamVal, astart, tstart, gstart, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0.9, TOLERANCE);
    EXPECT_NEAR(tauOut, 1  , TOLERANCE);
    EXPECT_NEAR(gOut  , 0.9, TOLERANCE);
}

TEST(InverseAddingDoubling, Test2) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 32;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.08531;
    constexpr T tsmeas = 0.77350;
    constexpr T tcmeas = 0.338341;

    T aOut, tauOut, gOut;
    T fixedParamVal = fixParam<T,M,N,fix>(0.0, nSlab, nSlideTop, nSlideBottom, tcmeas);
    T astart, gstart, tstart;
    Func<T,M,N,fix> toMinimize(fixedParamVal, nSlab, nSlideTop, nSlideBottom, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, astart, tstart, gstart);
    IAD<T,M,N,fix>(toMinimize, rsmeas, tsmeas, tcmeas, nSlab, nSlideTop, nSlideBottom, fixedParamVal, astart, tstart, gstart, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0.9, TOLERANCE);
    EXPECT_NEAR(tauOut, 1  , TOLERANCE);
    EXPECT_NEAR(gOut  , 0.9, TOLERANCE);
}

TEST(InverseAddingDoubling, Test3) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 16;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.5; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.06548;
    constexpr T tsmeas = 0.74409;
    constexpr T tcmeas = 0.124729;

    T aOut, tauOut, gOut;
    T fixedParamVal = fixParam<T,M,N,fix>(0.0, nSlab, nSlideTop, nSlideBottom, tcmeas);
    T astart, gstart, tstart;
    Func<T,M,N,fix> toMinimize(fixedParamVal, nSlab, nSlideTop, nSlideBottom, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, astart, tstart, gstart);
    IAD<T,M,N,fix>(toMinimize, rsmeas, tsmeas, tcmeas, nSlab, nSlideTop, nSlideBottom, fixedParamVal, astart, tstart, gstart, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0.9 , TOLERANCE);
    EXPECT_NEAR(tauOut, 2   , TOLERANCE);
    EXPECT_NEAR(gOut  , 0.99, TOLERANCE);
}

TEST(InverseAddingDoubling, G0) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 4;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.4; /// refraction index of slide
    constexpr T nSlideBottom = 1.4;
    constexpr T rsmeas = 0.38911;
    constexpr T tsmeas = 0.11869;
    constexpr T tcmeas = 0.006369;

    T aOut, tauOut, gOut;
    T fixedParamVal = fixParam<T,M,N,fix>(0.0, nSlab, nSlideTop, nSlideBottom, tcmeas);
    T astart, gstart, tstart;
    Func<T,M,N,fix> toMinimize(fixedParamVal, nSlab, nSlideTop, nSlideBottom, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, astart, tstart, gstart);
    IAD<T,M,N,fix>(toMinimize, rsmeas, tsmeas, tcmeas, nSlab, nSlideTop, nSlideBottom, fixedParamVal, astart, tstart, gstart, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0.95, TOLERANCE);
    EXPECT_NEAR(tauOut, 5   , TOLERANCE);
    EXPECT_NEAR(gOut  , 0   , TOLERANCE);
}

TEST(InverseAddingDoubling, A0_G0_Tau05) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 8;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.5; /// refraction index of sample
    constexpr T nSlideTop = 1.6; /// refraction index of slide
    constexpr T nSlideBottom = 1.6;
    constexpr T rsmeas = 0.07204;
    constexpr T tsmeas = 0.54314;
    constexpr T tcmeas = 0.543166;

    T aOut, tauOut, gOut;
    T fixedParamVal = fixParam<T,M,N,fix>(0.0, nSlab, nSlideTop, nSlideBottom, tcmeas);
    T astart, gstart, tstart;
    Func<T,M,N,fix> toMinimize(fixedParamVal, nSlab, nSlideTop, nSlideBottom, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, astart, tstart, gstart);
    IAD<T,M,N,fix>(toMinimize, rsmeas, tsmeas, tcmeas, nSlab, nSlideTop, nSlideBottom, fixedParamVal, astart, tstart, gstart, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0  , TOLERANCE);
    EXPECT_NEAR(tauOut, 0.5, TOLERANCE);
    /// When a = 0, no value of g is correct
    // EXPECT_NEAR(gOut, 0.9, 10 * TOLERANCE);
}

TEST(InverseAddingDoubling, A0_G0_Tau1) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int M = 32;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.3; /// refraction index of sample
    constexpr T nSlideTop = 1.4; /// refraction index of slide
    constexpr T nSlideBottom = 1.4;
    constexpr T rsmeas = 0.03278;
    constexpr T tsmeas = 0.34684;
    constexpr T tcmeas = 0.346838;

    T aOut, tauOut, gOut;
    T fixedParamVal = fixParam<T,M,N,fix>(0.0, nSlab, nSlideTop, nSlideBottom, tcmeas);
    T astart, gstart, tstart;
    Func<T,M,N,fix> toMinimize(fixedParamVal, nSlab, nSlideTop, nSlideBottom, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, astart, tstart, gstart);
    IAD<T,M,N,fix>(toMinimize, rsmeas, tsmeas, tcmeas, nSlab, nSlideTop, nSlideBottom, fixedParamVal, astart, tstart, gstart, aOut, tauOut, gOut);

    EXPECT_NEAR(aOut  , 0, TOLERANCE);
    EXPECT_NEAR(tauOut, 1, TOLERANCE);
    /// When a = 0, no value of g is correct
    // EXPECT_NEAR(gOut, 0, 5 * TOLERANCE);
}
