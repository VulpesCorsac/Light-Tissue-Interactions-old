#pragma once

#include "Medium.h"
#include "MonteCarlo.h"
#include "MCmultithread.h"
#include "Detector.h"

#include "../Physics/BugerLambert.h"

#include <gtest/gtest.h>

#include <optional>

using namespace Physics_NS;
using namespace std;

/************
 * FIXTURES *
 ************/

class General : public ::testing::TestWithParam<int> {
public:
    using T = double;

    static constexpr size_t Nz = 1E3;
    static constexpr size_t Nr = 1E4;
    static constexpr bool detector = 1;
    static constexpr T selectedRadius = 1E-1;

    static constexpr int singleThread = 1;
    static constexpr int multiThread  = 4;

    IntegratingSphere<T> sphereT{0.1, 0.01, 0.00};
    IntegratingSphere<T> sphereR{0.1, 0.01, 0.01};
    DetectorDistance<T>  dist   {0.0, 0.3 , 0.05};

public:
    template < typename T >
    struct TestResult {
        T SPECULAR_REFLECTION  = 0;
        T DIFFUSE_REFLECTION   = 0;
        T DIFFUSE_TRANSMISSION = 0;

        /// tolerance is relevant, not absolute
        T SPECULAR_REFLECTION_TOLERANCE  = 0;
        T DIFFUSE_REFLECTION_TOLERANCE   = 0;
        T DIFFUSE_TRANSMISSION_TOLERANCE = 0;
    };

    static constexpr T TOTAL_TOLERANCE = 1E-4;

public:
    void checkResult(const MCresults<T, Nz, Nr, detector>& res, const TestResult<T>& expected, optional<T> buger) const {
        EXPECT_NEAR(res.specularReflection , expected.SPECULAR_REFLECTION , expected.SPECULAR_REFLECTION_TOLERANCE  * expected.SPECULAR_REFLECTION );
        EXPECT_NEAR(res.diffuseReflection  , expected.DIFFUSE_REFLECTION  , expected.DIFFUSE_REFLECTION_TOLERANCE   * expected.DIFFUSE_REFLECTION  );
        EXPECT_NEAR(res.diffuseTransmission, expected.DIFFUSE_TRANSMISSION, expected.DIFFUSE_TRANSMISSION_TOLERANCE * expected.DIFFUSE_TRANSMISSION);

        if (buger)
            EXPECT_NEAR(res.diffuseTransmission, buger.value(), expected.DIFFUSE_TRANSMISSION_TOLERANCE * buger.value());

        EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1, TOTAL_TOLERANCE);
    }
};

class AbsorptionOnly : public General {
public:
    const Medium<T> tissue1 = Medium<T>::fromCoeffs(1.5, 1000, 0, 1E-3, 0.9); // MFP == D
    const Medium<T> tissue2 = Medium<T>::fromCoeffs(1.3,  100, 0, 1E-3, 0.5); // MFP == D * 10
    const Medium<T> tissue3 = Medium<T>::fromCoeffs(1.6, 1000, 0, 1E-2, 0.0); // MFP == D * 0.1
};

class SingleLayerAbsorptionOnly_MFP_IS_D : public AbsorptionOnly {
public:
    const Sample<T> sample{{tissue1}, 1, 1};
    static constexpr int photons = 1E6;
    const TestResult<T> EXPECTED{0.04, 0.004989, 0.3390, 1E-9, 0.01, 0.006};
    const T BUGER = BugerLambert<T>(1, 1.5, 1, 1);
};

class SingleLayerAbsorptionOnly_MFP_GT_D : public AbsorptionOnly {
public:
    const Sample<T> sample{{tissue2}, 1, 1};
    static constexpr int photons = 1E6;
    const TestResult<T> EXPECTED{0.0170132, 0.01346, 0.8743, 2E-6, 0.002, 0.001};
    const T BUGER = BugerLambert<T>(0.1, 1.3, 1, 1);
};

class SingleLayerAbsorptionOnly_MFP_LT_D : public AbsorptionOnly {
public:
    const Sample<T> sample{{tissue3}, 1, 1};
    static constexpr int photons = 5E6;
    const TestResult<T> EXPECTED{0.05325, 0, 0.00004069, 1E-4, 1E-9, 0.22};
    const T BUGER = BugerLambert<T>(10, 1.6, 1, 1);
};

class MultiLayerAbsorptionOnly_MFP_IS_D : public AbsorptionOnly {
public:
    const Medium<T> glass = Medium<T>::fromCoeffs(1.6, 0, 0, 1E-3, 0);
    const Sample<T> sample{{glass, tissue1, glass}, 1, 1};
    static constexpr int photons = 1E6;
    const TestResult<T> EXPECTED{0.05419, 0.00656, 0.3292, 6E-5, 0.01, 0.005};
    const T BUGER = BugerLambert<T>(1, 1.5, 1.6, 1.6);
};

class MultiLayerAbsorptionOnly_MFP_GT_D : public AbsorptionOnly {
public:
    const Medium<T> glass = Medium<T>::fromCoeffs(1.4, 0, 0, 1E-3, 0);
    const Sample<T> sample{{glass, tissue2, glass}, 1, 1};
    static constexpr int photons = 1E6;
    const TestResult<T> EXPECTED{0.02907, 0.02244, 0.853, 2E-4, 0.005, 0.002};
    const T BUGER = BugerLambert<T>(0.1, 1.3, 1.4, 1.4);
};

class MultiLayerAbsorptionOnly_MFP_LT_D : public AbsorptionOnly {
public:
    const Medium<T> glass = Medium<T>::fromCoeffs(1.65, 0, 0, 1E-3, 0);
    const Sample<T> sample{{glass, tissue3, glass}, 1, 1};
    static constexpr int photons = 2E7;
    const TestResult<T> EXPECTED{0.06037, 0, 0.00004008, 5E-5, 1E-9, 0.11};
    const T BUGER = BugerLambert<T>(10, 1.6, 1, 1);
};

class AbsorptionScattering : public General {
public:
    const Medium<T> tissue1 = Medium<T>::fromCoeffs(1.5, 100, 900,  1E-3, 0.9); // MFP == D
    const Medium<T> tissue2 = Medium<T>::fromCoeffs(1.3,  20,  80,  1E-3, 0.5); // MFP == D * 10
    const Medium<T> tissue3 = Medium<T>::fromCoeffs(1.6, 700, 300, 10E-3, 0.0); // MFP == D * 0.1
};

class SingleLayerAbsorptionScattering_MFP_IS_D : public AbsorptionScattering {
public:
    const Sample<T> sample{{tissue1}, 1, 1};
    static constexpr int photons = 5E5;
    const TestResult<T> EXPECTED{0.04, 0.0435, 0.767, 1E-9, 0.01, 0.002};
};

class SingleLayerAbsorptionScattering_MFP_GT_D : public AbsorptionScattering {
public:
    const Sample<T> sample{{tissue2}, 1, 1};
    const TestResult<T> EXPECTED{0.01701, 0.0272, 0.9206, 3E-4, 0.2, 0.001};
    static constexpr int photons = 5E5;
};

class SingleLayerAbsorptionScattering_MFP_LT_D : public AbsorptionScattering {
public:
    const Sample<T> sample{{tissue3}, 1, 1};
    const TestResult<T> EXPECTED{0.05325, 0.0175, 0.0000549, 1E-4, 0.013, 0.55};
    static constexpr int photons = 1E6;
};

class MultiLayerAbsorptionScattering_MFP_IS_D : public AbsorptionScattering {
public:
    const Medium<T> glass1 = Medium<T>::fromCoeffs(1.60, 0, 0, 1E-3, 0);
    const Medium<T> glass3 = Medium<T>::fromCoeffs(1.65, 0, 0, 1E-3, 0);
    const Sample<T> sample{{glass1, tissue1, glass3}, 1, 1};
    const TestResult<T> EXPECTED{0.05419, 0.05813, 0.7394, 6E-5, 0.05, 0.0012};
    static constexpr int photons = 1E6;
};

class MultiLayerAbsorptionScattering_MFP_GT_D : public AbsorptionScattering {
public:
    const Medium<T> glass = Medium<T>::fromCoeffs(1.4 , 0, 0, 1E-3, 0);
    const Sample<T> sample{{glass, tissue2, glass}, 1, 1};
    const TestResult<T> EXPECTED{0.02907, 0.03695, 0.8987, 2E-4, 0.009, 6E-4};
    static constexpr int photons = 1E6;
};

class MultiLayerAbsorptionScattering_MFP_LT_D : public AbsorptionScattering {
public:
    const Medium<T> glass = Medium<T>::fromCoeffs(1.65, 0, 0, 1E-3, 0);
    const Sample<T> sample{{glass, tissue3, glass}, 1, 1};
    const TestResult<T> EXPECTED{0.06037, 0.01718, 0.0000541, 5E-5, 0.012, 0.37};
    static constexpr int photons = 1E6;
};

/*********
 * TESTS *
 *********/

#define TESTING_VALUES ::testing::Values(0,1,2,3,4)

#define TEST_MC(TESTNAME,BUGER_VALUE)                                          \
TEST_P(TESTNAME, SingleThread) {                                               \
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample,                        \
                                                photons,                       \
                                                sample.getTotalThickness(),    \
                                                selectedRadius,                \
                                                sphereR,                       \
                                                sphereT,                       \
                                                dist).CalculateResult(),       \
                EXPECTED,                                                      \
                BUGER_VALUE);                                                  \
}                                                                              \
                                                                               \
TEST_P(TESTNAME, MultiThread) {                                                \
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample,                     \
                                                   photons,                    \
                                                   multiThread,                \
                                                   sample.getTotalThickness(), \
                                                   selectedRadius,             \
                                                   sphereR,                    \
                                                   sphereT,                    \
                                                   dist),                      \
                EXPECTED,                                                      \
                BUGER_VALUE);                                                  \
}                                                                              \
                                                                               \
INSTANTIATE_TEST_SUITE_P(MC, TESTNAME, TESTING_VALUES);

TEST_MC(SingleLayerAbsorptionOnly_MFP_IS_D      , BUGER);
TEST_MC(SingleLayerAbsorptionOnly_MFP_GT_D      , BUGER);
TEST_MC(SingleLayerAbsorptionOnly_MFP_LT_D      , BUGER);
TEST_MC(MultiLayerAbsorptionOnly_MFP_IS_D       , BUGER);
TEST_MC(MultiLayerAbsorptionOnly_MFP_GT_D       , BUGER);
TEST_MC(MultiLayerAbsorptionOnly_MFP_LT_D       , BUGER);
TEST_MC(SingleLayerAbsorptionScattering_MFP_IS_D, nullopt);
TEST_MC(SingleLayerAbsorptionScattering_MFP_GT_D, nullopt);
TEST_MC(SingleLayerAbsorptionScattering_MFP_LT_D, nullopt);
TEST_MC(MultiLayerAbsorptionScattering_MFP_IS_D , nullopt);
TEST_MC(MultiLayerAbsorptionScattering_MFP_GT_D , nullopt);
TEST_MC(MultiLayerAbsorptionScattering_MFP_LT_D , nullopt);
