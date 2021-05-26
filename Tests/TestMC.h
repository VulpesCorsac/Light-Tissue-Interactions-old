#pragma once

#include "../MC/Medium.h"
#include "../MC/BugerLambert.h"
#include "../MC/MonteCarlo.h"
#include "../MC/MCmultithread.h"

#include <gtest/gtest.h>

#include <optional>

using namespace std;

/************
 * FIXTURES *
 ************/

class MC_General : public ::testing::TestWithParam<int> {
public:
    using T = double;

    static constexpr size_t Nz = 1e3;
    static constexpr size_t Nr = 1e4;
    static constexpr bool detector = 1;
    static constexpr T selectedRadius = 1e-1;

    static constexpr int single_thread = 1;
    static constexpr int multi_thread  = 4;

    const IntegratingSphere<T> SphereT(0.1, 0.01, 0.00);
    const IntegratingSphere<T> SphereR(0.1, 0.01, 0.01);
    const DetectorDistances<T> dist;
    dist.maxDist = 0.3;
    dist.minDist = 0.0;
    dist.stepSize = 0.05;

public:
    template < typename T >
    struct TestResult {
        T SPECULAR_REFLECTION  = 0;
        T DIFFUSE_REFLECTION   = 0;
        T DIFFUSE_TRANSMISSION = 0;

        T SPECULAR_REFLECTION_TOLERANCE  = 0;
        T DIFFUSE_REFLECTION_TOLERANCE   = 0;
        T DIFFUSE_TRANSMISSION_TOLERANCE = 0;
    };

    static constexpr T TOTAL_TOLERANCE = 1e-4;

public:
    void checkResult(const MCresults<T, Nz, Nr, detector>& res, const TestResult<T>& expected, std::optional<T> buger) const {
        EXPECT_NEAR(res.specularReflection , expected.SPECULAR_REFLECTION , expected.SPECULAR_REFLECTION_TOLERANCE);
        EXPECT_NEAR(res.diffuseReflection  , expected.DIFFUSE_REFLECTION  , expected.DIFFUSE_REFLECTION_TOLERANCE);
        EXPECT_NEAR(res.diffuseTransmission, expected.DIFFUSE_TRANSMISSION, expected.DIFFUSE_TRANSMISSION_TOLERANCE);

        if (buger)
            EXPECT_NEAR(res.diffuseTransmission, buger.value(), expected.DIFFUSE_TRANSMISSION_TOLERANCE);

        EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1,  TOTAL_TOLERANCE);
    }
};

class MC_AbsorptionOnly : public MC_General {
public:
    const Medium<T> tissue1{1.5, 1000, 0, 1e-3, 0.9}; // MFP == D
    const Medium<T> tissue2{1.3,  100, 0, 1e-3, 0.5}; // MFP == D * 10
    const Medium<T> tissue3{1.6, 1000, 0, 1e-2, 0.0}; // MFP == D * 0.1
};

class MC_SingleLayerAbsorptionOnly_MFP_IS_D : public MC_AbsorptionOnly {
public:
    const Sample<T> sample{{tissue1}, 1, 1};
    static constexpr int photons = 1e6;
    const TestResult<T> EXPECTED{0.04, 0.004989, 0.3390, 1e-9, 5e-5, 2e-3};
    const T BUGER = BugerLambert<T>(1, 1.5, 1, 1);
};

class MC_SingleLayerAbsorptionOnly_MFP_GREATER_THAN_D : public MC_AbsorptionOnly {
public:
    const Sample<T> sample{{tissue2}, 1, 1};
    static constexpr int photons = 1e6;
    const TestResult<T> EXPECTED{0.0170132, 0.01346, 0.8743, 4e-8, 3e-5, 1e-3};
    const T BUGER = BugerLambert<T>(0.1, 1.3, 1, 1);
};

class MC_SingleLayerAbsorptionOnly_MFP_LESS_THAN_D : public MC_AbsorptionOnly {
public:
    const Sample<T> sample{{tissue3}, 1, 1};
    static constexpr int photons = 5e6;
    const TestResult<T> EXPECTED{0.05325, 0, 0.00004069, 5e-6, 1e-9, 9e-6};
    const T BUGER = BugerLambert<T>(10, 1.6, 1, 1);
};

class MC_MultiLayerAbsorptionOnly_MFP_IS_D : public MC_AbsorptionOnly {
public:
    const Medium<T> glass{1.6 , 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue1, glass}, 1, 1};
    static constexpr int photons = 1e6;
    const TestResult<T> EXPECTED{0.05419, 0.00656, 0.3292, 3e-6, 6e-5, 1.5e-3};
    const T BUGER = BugerLambert<T>(1, 1.5, 1.6, 1.6);
};

class MC_MultiLayerAbsorptionOnly_MFP_GREATER_THAN_D : public MC_AbsorptionOnly {
public:
    const Medium<T> glass{1.4 , 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue2, glass}, 1, 1};
    static constexpr int photons = 1e6;
    const TestResult<T> EXPECTED{0.02907, 0.02244, 0.853, 5e-6, 1e-4, 1.5e-3};
    const T BUGER = BugerLambert<T>(0.1, 1.3, 1.4, 1.4);
};

class MC_MultiLayerAbsorptionOnly_MFP_LESS_THAN_D : public MC_AbsorptionOnly {
public:
    const Medium<T> glass{1.65, 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue3, glass}, 1, 1};
    static constexpr int photons = 2e7;
    const TestResult<T> EXPECTED{0.06037, 0, 0.00004008, 3e-6, 1e-9, 4e-6};
    const T BUGER = BugerLambert<T>(10, 1.6, 1, 1);
};

class MC_AbsorptionScattering : public MC_General {
public:
    const Medium<T> tissue1{1.5, 100, 900,  1e-3, 0.9}; // MFP == D
    const Medium<T> tissue2{1.3,  20,  80,  1e-3, 0.5}; // MFP == D * 10
    const Medium<T> tissue3{1.6, 700, 300, 10e-3, 0.0}; // MFP == D * 0.1
};

class MC_SingleLayerAbsorptionScattering_MFP_IS_D : public MC_AbsorptionScattering {
public:
    const Sample<T> sample{{tissue1}, 1, 1};
    const TestResult<T> EXPECTED{0.04, 0.0435, 0.767, 1e-9, 4e-4, 1.5e-3};
    static constexpr int photons = 5e5;
};

class MC_SingleLayerAbsorptionScattering_MFP_GREATER_THAN_D : public MC_AbsorptionScattering {
public:
    const Sample<T> sample{{tissue2}, 1, 1};
    const TestResult<T> EXPECTED{0.01701, 0.0272, 0.9206, 4e-6, 5e-4, 9e-4};
    static constexpr int photons = 5e5;
};

class MC_SingleLayerAbsorptionScattering_MFP_LESS_THAN_D : public MC_AbsorptionScattering {
public:
    const Sample<T> sample{{tissue3}, 1, 1};
    const TestResult<T> EXPECTED{0.05325, 0.0175, 0.0000549, 5e-6, 2e-4, 3e-5};
    static constexpr int photons = 1e6;
};

class MC_MultiLayerAbsorptionScattering_MFP_IS_D : public MC_AbsorptionScattering {
public:
    const Medium<T> glass1{1.6 , 0, 0, 1e-3, 0};
    const Medium<T> glass3{1.65, 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass1, tissue1, glass3}, 1, 1};
    const TestResult<T> EXPECTED{0.05419, 0.05813, 0.7394, 3e-6, 3e-3, 7e-4};
    static constexpr int photons = 1e6;
};

class MC_MultiLayerAbsorptionScattering_MFP_GREATER_THAN_D : public MC_AbsorptionScattering {
public:
    const Medium<T> glass{1.4 , 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue2, glass}, 1, 1};
    const TestResult<T> EXPECTED{0.02907, 0.03695, 0.8987, 5e-6, 3e-4, 5e-4};
    static constexpr int photons = 1e6;
};

class MC_MultiLayerAbsorptionScattering_MFP_LESS_THAN_D : public MC_AbsorptionScattering {
public:
    const Medium<T> glass{1.65, 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue3, glass}, 1, 1};
    const TestResult<T> EXPECTED{0.06037, 0.01718, 0.0000541, 3e-6, 2e-4, 2e-5};
    static constexpr int photons = 1e6;
};

/*********
 * TESTS *
 *********/

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_IS_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, BUGER);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_IS_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, BUGER);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_GREATER_THAN_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, BUGER);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_GREATER_THAN_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, BUGER);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_LESS_THAN_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, BUGER);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_LESS_THAN_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, BUGER);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_IS_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, BUGER);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_IS_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, BUGER);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_GREATER_THAN_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, BUGER);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_GREATER_THAN_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, BUGER);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_LESS_THAN_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, BUGER);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_LESS_THAN_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, BUGER);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_IS_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, std::nullopt);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_IS_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, std::nullopt);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_GREATER_THAN_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, std::nullopt);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_GREATER_THAN_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, std::nullopt);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_LESS_THAN_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, std::nullopt);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_LESS_THAN_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, std::nullopt);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_IS_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, std::nullopt);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_IS_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, std::nullopt);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_GREATER_THAN_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, std::nullopt);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_GREATER_THAN_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, std::nullopt);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_LESS_THAN_D, SingleThread) {
    checkResult(MonteCarlo<T, Nz, Nr, detector>(sample, photons, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist).CalculateResult(), EXPECTED, std::nullopt);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_LESS_THAN_D, MultiThread) {
    checkResult(MCmultithread<T, Nz, Nr, detector>(sample, photons, multi_thread, sample.getTotalThickness(), selectedRadius, SphereR, SphereT, dist), EXPECTED, std::nullopt);
}

/*************
 * INSTANCES *
 *************/

#define TESTING_VALUES ::testing::Values(0,1,2,3,4)
INSTANTIATE_TEST_CASE_P(MC, MC_SingleLayerAbsorptionOnly_MFP_IS_D                , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_SingleLayerAbsorptionOnly_MFP_GREATER_THAN_D      , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_SingleLayerAbsorptionOnly_MFP_LESS_THAN_D         , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_MultiLayerAbsorptionOnly_MFP_IS_D                 , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_MultiLayerAbsorptionOnly_MFP_GREATER_THAN_D       , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_MultiLayerAbsorptionOnly_MFP_LESS_THAN_D          , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_SingleLayerAbsorptionScattering_MFP_IS_D          , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_SingleLayerAbsorptionScattering_MFP_GREATER_THAN_D, TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_SingleLayerAbsorptionScattering_MFP_LESS_THAN_D   , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_MultiLayerAbsorptionScattering_MFP_IS_D           , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_MultiLayerAbsorptionScattering_MFP_GREATER_THAN_D , TESTING_VALUES);
INSTANTIATE_TEST_CASE_P(MC, MC_MultiLayerAbsorptionScattering_MFP_LESS_THAN_D    , TESTING_VALUES);
