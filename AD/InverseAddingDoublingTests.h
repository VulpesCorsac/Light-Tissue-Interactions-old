#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "../Inverse/InverseProblem.h"
#include "../Inverse/StartingPoints.h"

#include <gtest/gtest.h>

/// TODO: make some statistics as in MC tests
/// TODO: extract tolerance

using namespace AddingDoubling_NS;
using namespace Inverse_NS;

TEST(InverseAddingDoubling, Test1) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 4;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.0862354 - 0.0410959;
    constexpr T tsmeas = 0.76446;
    constexpr T tcmeas = 0.338341;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.9, TOLERANCE);
    EXPECT_NEAR(tauOut, 1  , TOLERANCE);
    EXPECT_NEAR(gOut  , 0.9, TOLERANCE);
}

TEST(InverseAddingDoubling, Test2) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 32;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;

    constexpr T rsmeas = 0.08531 - 0.0410959;
    constexpr T tsmeas = 0.77350;
    constexpr T tcmeas = 0.338341;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.9, TOLERANCE);
    EXPECT_NEAR(tauOut, 1  , TOLERANCE);
    EXPECT_NEAR(gOut  , 0.9, TOLERANCE);
}

TEST(InverseAddingDoubling, Test3) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 16;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.5; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.06548 - 0.04;
    constexpr T tsmeas = 0.74409;
    constexpr T tcmeas = 0.124729;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.9 , TOLERANCE);
    EXPECT_NEAR(tauOut, 2   , TOLERANCE);
    EXPECT_NEAR(gOut  , 0.99, TOLERANCE);
}

TEST(InverseAddingDoubling, G0) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 4;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.4; /// refraction index of slide
    constexpr T nSlideBottom = 1.4;
    constexpr T rsmeas = 0.38911 - 0.02778;
    constexpr T tsmeas = 0.11869;
    constexpr T tcmeas = 0.006369;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.95, TOLERANCE);
    EXPECT_NEAR(tauOut, 5   , TOLERANCE);
    EXPECT_NEAR(gOut  , 0   , TOLERANCE);
}

TEST(InverseAddingDoubling, A0_G0_Tau05) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 8;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.5; /// refraction index of sample
    constexpr T nSlideTop = 1.6; /// refraction index of slide
    constexpr T nSlideBottom = 1.6;
    constexpr T rsmeas = 0.07204 - 0.05419;
    constexpr T tsmeas = 0.54314;
    constexpr T tcmeas = 0.543166;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0  , TOLERANCE);
    EXPECT_NEAR(tauOut, 0.5, TOLERANCE);
    /// When a = 0, no value of g is correct
    // EXPECT_NEAR(gOut, 0.9, 10 * TOLERANCE);
}

TEST(InverseAddingDoubling, A0_G0_Tau1) {
    using T = float;

    constexpr double TOLERANCE = 1e-4;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 32;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.3; /// refraction index of sample
    constexpr T nSlideTop = 1.4; /// refraction index of slide
    constexpr T nSlideBottom = 1.4;
    constexpr T rsmeas = 0.03278 - 0.02907;
    constexpr T tsmeas = 0.34684;
    constexpr T tcmeas = 0.346838;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0, TOLERANCE);
    EXPECT_NEAR(tauOut, 1, TOLERANCE);
    /// When a = 0, no value of g is correct
    // EXPECT_NEAR(gOut, 0, 5 * TOLERANCE);
}

TEST(InverseAddingDoubling, TauMin1) {
    using T = float;

    constexpr double TOLERANCE = 1e-2;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 32;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::G;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.08624 - 0.0410959;
    constexpr T tsmeas = 0.76446;
    constexpr T tcmeas = 0.338341;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.9,      TOLERANCE);
    EXPECT_NEAR(tauOut, 1.0, 10 * TOLERANCE);
    EXPECT_NEAR(gOut  , 0.9,      TOLERANCE);
}

TEST(InverseAddingDoubling, TauMin2) {
    using T = float;

    constexpr double TOLERANCE = 1e-2;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 32;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::G;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.4; /// refraction index of slide
    constexpr T nSlideBottom = 1.4;
    constexpr T rsmeas = 0.38911 - 0.02778;
    constexpr T tsmeas = 0.11869;
    constexpr T tcmeas = 0.006369;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.95,      TOLERANCE);
    EXPECT_NEAR(tauOut, 5.0, 10 * TOLERANCE);
    EXPECT_NEAR(gOut  , 0.0,      TOLERANCE);
}

TEST(InverseAddingDoubling, TauMin3) {
    using T = float;

    constexpr double TOLERANCE = 1e-2;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 32;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::G;

    constexpr T nSlab = 1.5; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.06548 - 0.04;
    constexpr T tsmeas = 0.74409;
    constexpr T tcmeas = 0.124729;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.9,      TOLERANCE);
    EXPECT_NEAR(tauOut, 2.0, 10 * TOLERANCE);
    EXPECT_NEAR(gOut  , 0.99,      TOLERANCE);
}

TEST(InverseAddingDoubling, TauMin4) {
    using T = float;

    constexpr double TOLERANCE = 1e-2;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1;
    constexpr int M = 32;
    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::G;

    constexpr T nSlab = 1.5; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.0630203 - 0.04;
    constexpr T tsmeas = 0.699568;
    constexpr T tcmeas = 0.559308;
    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.5,      TOLERANCE);
    EXPECT_NEAR(tauOut, 0.5, 10 * TOLERANCE);
    EXPECT_NEAR(gOut  , 0.9,      TOLERANCE);
}
/*
TEST(InverseAddingDoubling, 3NMin1) {
    using T = float;

    constexpr double TOLERANCE = 5e-2;

    constexpr int M = 32;
    constexpr int N = 3; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::G;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.08624;
    constexpr T tsmeas = 0.76446;
    constexpr T tcmeas = 0.338341;

   Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.9, TOLERANCE);
    EXPECT_NEAR(tauOut, 1.0, TOLERANCE);
    EXPECT_NEAR(gOut  , 0.9, TOLERANCE);
}

TEST(InverseAddingDoubling, 3NMin2) {
    using T = float;

    constexpr double TOLERANCE = 5e-2;

    constexpr int M = 32;
    constexpr int N = 3; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::G;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.4; /// refraction index of slide
    constexpr T nSlideBottom = 1.4;
    constexpr T rsmeas = 0.38911;
    constexpr T tsmeas = 0.11869;
    constexpr T tcmeas = 0.006369;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.95, TOLERANCE);
    EXPECT_NEAR(tauOut, 5.0,  TOLERANCE);
    EXPECT_NEAR(gOut  , 0.0,  TOLERANCE);
}

TEST(InverseAddingDoubling, 3NMin3) {
    using T = float;

    constexpr double TOLERANCE = 5e-2;

    constexpr int M = 32;
    constexpr int N = 3; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::G;

    constexpr T nSlab = 1.5; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;
    constexpr T rsmeas = 0.06548;
    constexpr T tsmeas = 0.74409;
    constexpr T tcmeas = 0.124729;

    Meduim<T> emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    Meduim<T> glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    std::vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, rsmeas, tsmeas, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tOut, gOut, ModellingMethod::AD);
    std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    EXPECT_NEAR(aOut  , 0.9,  TOLERANCE);
    EXPECT_NEAR(tauOut, 2.0,  TOLERANCE);
    EXPECT_NEAR(gOut  , 0.99, TOLERANCE);
}*/
