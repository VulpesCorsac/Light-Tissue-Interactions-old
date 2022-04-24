#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "../Inverse/InverseProblem.h"
#include "../Inverse/StartingPoints.h"
#include "../MC/Medium.h"
#include "../MC/Sample.h"

#include <gtest/gtest.h>

/// TODO: make some statistics as in MC tests
/// TODO: extract tolerance

using namespace AddingDoubling_NS;
using namespace Inverse_NS;
using namespace std;

template < typename T, size_t Nz, size_t Nr, bool detector, size_t M, size_t N, FixedParameter fix >
class TestIAD {
public:
    TestIAD(const T& nSlabNew, const T& nSlideTopNew, const T& nSlideBottomNew,
            const T& rMeasNew, const T& tMeasNew, const T& tcMeasNew, const T& gNew) EXCEPT_INPUT_PARAMS
            : nSlab(nSlabNew)
            , nSlideTop(nSlideTopNew)
            , nSlideBottom(nSlideBottomNew)
            , rMeas(rMeasNew)
            , tMeas(tMeasNew)
            , tcMeas(tcMeasNew)
            , gVal(gNew) {
        setValues();
        calc();
    }

    void setValues() {
        Rd = {{0.0, rMeas}};
        Td = {{0.0, tMeas}};

        SphereT = IntegratingSphere<T>(0.0508, 0.0125, 0.0);
        SphereR = IntegratingSphere<T>(0.0508, 0.0125, 0.0125);
        distances.max  = 0.0;
        distances.min  = 0.0;
        distances.step = 0.0;
        source = LightSource<T>(0.0, SourceType::Point);

        emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
        glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
        glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
        vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
        emptySample = Sample<T>(layers);
    }

    void calc() {
        T fixedParamVal = fixParam<T,fix>(gVal, emptySample, tcMeas);
        Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, 1000, 1,
                                                  emptySample.getTotalThickness(), 10E-2, SphereR, SphereT,
                                                  distances, Rd, Td, tcMeas, source);
        startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
        toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
        std::cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << std::endl;
    }

    T getAout() const noexcept { return aOut;   }
    T getTout() const noexcept { return tauOut; }
    T getGout() const noexcept { return gOut;   }

protected:
    T nSlab, nSlideTop, nSlideBottom;
    T rMeas, tMeas, tcMeas, gVal;
    vector<pair<T,T>> Rd, Td;
    IntegratingSphere<T> SphereR, SphereT;
    DetectorDistance<T> distances;
    LightSource<T> source;
    Medium<T> emptyTissue, glassTop, glassBottom;
    Sample<T> emptySample;
    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;
};

TEST(HealthCheck_InverseAddingDoubling, Test1) {
    constexpr double TOLERANCE = 1e-4;
    TestIAD<float, 1000, 10000, 1, 4, 2, FixedParameter::Tau> test(1.4, 1.5, 1.5, 0.0862354 - 0.0410959, 0.76446, 0.338341, 0.0);
    EXPECT_NEAR(test.getAout(), 0.9, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9, TOLERANCE);
}

TEST(HealthCheck_InverseAddingDoubling, Test2) {
    constexpr double TOLERANCE = 1e-4;
    TestIAD<float, 1000, 10000, 1, 32, 2, FixedParameter::Tau> test(1.4, 1.5, 1.5, 0.08531 - 0.0410959, 0.77350, 0.338341, 0.0);
    EXPECT_NEAR(test.getAout(), 0.9, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9, TOLERANCE);
}

TEST(HealthCheck_InverseAddingDoubling, Test3) {
    constexpr double TOLERANCE = 1e-4;
    TestIAD<float, 1000, 10000, 1, 16, 2, FixedParameter::Tau> test(1.5, 1.5, 1.5, 0.06548 - 0.04, 0.74409, 0.124729, 0.0);
    EXPECT_NEAR(test.getAout(), 0.9 , TOLERANCE);
    EXPECT_NEAR(test.getTout(), 2   , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.99, TOLERANCE);
}

TEST(HealthCheck_InverseAddingDoubling, G0) {
    constexpr double TOLERANCE = 1e-4;
    TestIAD<float, 1000, 10000, 1, 4, 2, FixedParameter::Tau> test(1.4, 1.4, 1.4, 0.38911 - 0.02778, 0.11869, 0.006369, 0.0);
    EXPECT_NEAR(test.getAout(), 0.95, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 5   , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0   , TOLERANCE);
}

TEST(HealthCheck_InverseAddingDoubling, A0_G0_Tau05) {
    constexpr double TOLERANCE = 1e-4;
    TestIAD<float, 1000, 10000, 1, 8, 2, FixedParameter::Tau> test(1.5, 1.6, 1.6, 0.07204 - 0.05419, 0.54314, 0.543166, 0.0);
    EXPECT_NEAR(test.getAout(), 0  , TOLERANCE);
    EXPECT_NEAR(test.getTout(), 0.5, TOLERANCE);
    /// When a = 0, no value of g is correct
    // EXPECT_NEAR(test.getGout(), 0   , TOLERANCE);
}

TEST(HealthCheck_InverseAddingDoubling, A0_G0_Tau1) {
    constexpr double TOLERANCE = 1e-4;
    TestIAD<float, 1000, 10000, 1, 32, 2, FixedParameter::Tau> test(1.3, 1.4, 1.4, 0.03278 - 0.02907, 0.34684, 0.346838, 0.0);
    EXPECT_NEAR(test.getAout(), 0, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1, TOLERANCE);
    /// When a = 0, no value of g is correct
    //EXPECT_NEAR(test.getGout(), 0   , TOLERANCE);
}

TEST(HealthCheck_InverseAddingDoubling, TauMin1) {
    constexpr double TOLERANCE = 1e-2;
    TestIAD<float, 1000, 10000, 1, 32, 2, FixedParameter::G> test(1.4, 1.5, 1.5, 0.08624 - 0.0410959, 0.76446, 0.338341, 0.9);
    EXPECT_NEAR(test.getAout(), 0.9,      TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1.0, 10 * TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9,      TOLERANCE);
}

TEST(HealthCheck_InverseAddingDoubling, TauMin2) {
    constexpr double TOLERANCE = 1e-2;
    TestIAD<float, 1000, 10000, 1, 32, 2, FixedParameter::G> test(1.4, 1.4, 1.4, 0.38911 - 0.02778, 0.11869, 0.006369, 0.0);
    EXPECT_NEAR(test.getAout(), 0.95,      TOLERANCE);
    EXPECT_NEAR(test.getTout(), 5.0 , 10 * TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.0 ,      TOLERANCE);
}

TEST(HealthCheck_InverseAddingDoubling, TauMin3) {
    constexpr double TOLERANCE = 1e-2;
    TestIAD<float, 1000, 10000, 1, 32, 2, FixedParameter::G> test(1.5, 1.5, 1.5, 0.06548 - 0.04, 0.74409, 0.124729, 0.99);
    EXPECT_NEAR(test.getAout(), 0.9 ,      TOLERANCE);
    EXPECT_NEAR(test.getTout(), 2.0 , 10 * TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.99,      TOLERANCE);
}

TEST(HealthCheck_InverseAddingDoubling, TauMin4) {
    constexpr double TOLERANCE = 1e-2;
    TestIAD<float, 1000, 10000, 1, 32, 2, FixedParameter::G> test(1.5, 1.5, 1.5, 0.0630203 - 0.04, 0.699568, 0.559308, 0.9);
    EXPECT_NEAR(test.getAout(), 0.5,      TOLERANCE);
    EXPECT_NEAR(test.getTout(), 0.5, 10 * TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9,      TOLERANCE);
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
}
//*/
