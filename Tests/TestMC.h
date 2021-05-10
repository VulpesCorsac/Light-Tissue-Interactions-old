#pragma once

#include "../MC/Medium.h"
#include "../MC/BugerLambert.h"
#include "../MC/MonteCarlo.h"
#include "../MC/MCmultithread.h"

#include <gtest/gtest.h>

using namespace std;

/************
 * FIXTURES *
 ************/

class MC_General : public ::testing::TestWithParam<int> {
public:
    using T = double;

    static constexpr size_t Nz = 1e3;
    static constexpr size_t Nr = 1e4;
    static constexpr T selectedRadius = 1e-1;

    static constexpr T TOTAL_TOLERANCE = 1e-4;

    static constexpr int single_thread = 1;
    static constexpr int multi_thread  = 4;
};

class MC_AbsorptionOnly : public MC_General {
public:
    const Medium<T> tissue1{1.5, 1000, 0, 1e-3, 0.9}; // MFP == D
    const Medium<T> tissue2{1.3,  100, 0, 1e-3, 0.5}; // MFP == 10*D
    const Medium<T> tissue3{1.6, 1000, 0, 1e-2, 0.0}; // MFP == 0.1*D, it will need more photons
};

class MC_SingleLayerAbsorptionOnly : public MC_AbsorptionOnly {
public:
    const vector<Medium<T>> layer1{tissue1};
    const vector<Medium<T>> layer2{tissue2};
    const vector<Medium<T>> layer3{tissue3};

    const Sample<T> sample1{layer1, 1, 1};
    const Sample<T> sample2{layer2, 1, 1};
    const Sample<T> sample3{layer3, 1, 1};
};

class MC_SingleLayerAbsorptionOnly_MFP_IS_D : public MC_SingleLayerAbsorptionOnly {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.04;
    static constexpr T DIFFUSE_REFLECTION   = 0.004989;
    static constexpr T DIFFUSE_TRANSMISSION = 0.3390;

    static constexpr int photons = 1e6;

    const T BUGER = BugerLambert<T>(1, 1.5, 1, 1);
};

class MC_SingleLayerAbsorptionOnly_MFP_GREATER_THAN_D : public MC_SingleLayerAbsorptionOnly {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.0170132;
    static constexpr T DIFFUSE_REFLECTION   = 0.01346;
    static constexpr T DIFFUSE_TRANSMISSION = 0.8743;

    static constexpr int photons = 1e6;

    const T BUGER = BugerLambert<T>(0.1, 1.3, 1, 1);
};

class MC_SingleLayerAbsorptionOnly_MFP_LESS_THAN_D : public MC_SingleLayerAbsorptionOnly {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.05325;
    static constexpr T DIFFUSE_REFLECTION   = 0;
    static constexpr T DIFFUSE_TRANSMISSION = 0.00004069;

    static constexpr int photons = 5e6;

    const T BUGER = BugerLambert<T>(10, 1.6, 1, 1);
};

class MC_MultiLayerAbsorptionOnly : public MC_AbsorptionOnly {
public:
    const Medium<T> glass1{1.6 , 0, 0, 1e-3, 0};
    const Medium<T> glass2{1.4 , 0, 0, 1e-3, 0};
    const Medium<T> glass3{1.65, 0, 0, 1e-3, 0};

    const vector<Medium<T>> layer1{glass1, tissue1, glass1};
    const vector<Medium<T>> layer2{glass2, tissue2, glass2};
    const vector<Medium<T>> layer3{glass3, tissue3, glass3};

    const Sample<T> sample1{layer1, 1, 1};
    const Sample<T> sample2{layer2, 1, 1};
    const Sample<T> sample3{layer3, 1, 1};
};

class MC_MultiLayerAbsorptionOnly_MFP_IS_D : public MC_MultiLayerAbsorptionOnly {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.05419;
    static constexpr T DIFFUSE_REFLECTION   = 0.00656;
    static constexpr T DIFFUSE_TRANSMISSION = 0.3292;

    static constexpr int photons = 1e6;

    const T BUGER = BugerLambert<T>(1, 1.5, 1.6, 1.6);
};

class MC_MultiLayerAbsorptionOnly_MFP_GREATER_THAN_D : public MC_MultiLayerAbsorptionOnly {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.02907;
    static constexpr T DIFFUSE_REFLECTION   = 0.02244;
    static constexpr T DIFFUSE_TRANSMISSION = 0.853;

    static constexpr int photons = 1e6;

    const T BUGER = BugerLambert<T>(0.1, 1.3, 1.4, 1.4);
};

class MC_MultiLayerAbsorptionOnly_MFP_LESS_THAN_D : public MC_MultiLayerAbsorptionOnly {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.06037;
    static constexpr T DIFFUSE_REFLECTION   = 0;
    static constexpr T DIFFUSE_TRANSMISSION = 0.00004008;

    static constexpr int photons = 2e7;

    const T BUGER = BugerLambert<T>(10, 1.6, 1, 1);
};

class MC_AbsorptionScattering : public MC_General {
public:
    const Medium<T> tissue1{1.5, 100, 900,  1e-3, 0.9}; // MFP == D
    const Medium<T> tissue2{1.3,  20,  80,  1e-3, 0.5}; // MFP == 10*D
    const Medium<T> tissue3{1.6, 700, 300, 10e-3, 0.0}; // MFP == 0.1*D, it will need more photons
};

class MC_SingleLayerAbsorptionScattering : public MC_AbsorptionScattering {
public:
    const vector<Medium<T>> layer1{tissue1};
    const vector<Medium<T>> layer2{tissue2};
    const vector<Medium<T>> layer3{tissue3};

    const Sample<T> sample1{layer1, 1, 1};
    const Sample<T> sample2{layer2, 1, 1};
    const Sample<T> sample3{layer3, 1, 1};
};

class MC_SingleLayerAbsorptionScattering_MFP_IS_D : public MC_SingleLayerAbsorptionScattering {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.04;
    static constexpr T DIFFUSE_REFLECTION   = 0.0435;
    static constexpr T DIFFUSE_TRANSMISSION = 0.767;

    static constexpr int photons = 5e5;
};

class MC_SingleLayerAbsorptionScattering_MFP_GREATER_THAN_D : public MC_SingleLayerAbsorptionScattering {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.01701;
    static constexpr T DIFFUSE_REFLECTION   = 0.0272;
    static constexpr T DIFFUSE_TRANSMISSION = 0.9206;

    static constexpr int photons = 5e5;
};

class MC_SingleLayerAbsorptionScattering_MFP_LESS_THAN_D : public MC_SingleLayerAbsorptionScattering {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.05325;
    static constexpr T DIFFUSE_REFLECTION   = 0.0175;
    static constexpr T DIFFUSE_TRANSMISSION = 0.0000549;

    static constexpr int photons = 1e6;
};

class MC_MultiLayerAbsorptionScattering : public MC_AbsorptionScattering {
public:
    const Medium<T> glass1{1.6 , 0, 0, 1e-3, 0};
    const Medium<T> glass2{1.4 , 0, 0, 1e-3, 0};
    const Medium<T> glass3{1.65, 0, 0, 1e-3, 0};

    const vector<Medium<T>> layer1{glass1, tissue1, glass3}; /// TODO: recalculate for glass 1
    const vector<Medium<T>> layer2{glass2, tissue2, glass2};
    const vector<Medium<T>> layer3{glass3, tissue3, glass3};

    const Sample<T> sample1{layer1, 1, 1};
    const Sample<T> sample2{layer2, 1, 1};
    const Sample<T> sample3{layer3, 1, 1};
};

class MC_MultiLayerAbsorptionScattering_MFP_IS_D : public MC_MultiLayerAbsorptionScattering {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.05419;
    static constexpr T DIFFUSE_REFLECTION   = 0.05813;
    static constexpr T DIFFUSE_TRANSMISSION = 0.7394;

    static constexpr int photons = 1e6;
};

class MC_MultiLayerAbsorptionScattering_MFP_GREATER_THAN_D : public MC_MultiLayerAbsorptionScattering {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.02907;
    static constexpr T DIFFUSE_REFLECTION   = 0.03695;
    static constexpr T DIFFUSE_TRANSMISSION = 0.8987;

    static constexpr int photons = 1e6;
};

class MC_MultiLayerAbsorptionScattering_MFP_LESS_THAN_D : public MC_MultiLayerAbsorptionScattering {
public:
    static constexpr T SPECULAR_REFLECTION  = 0.06037;
    static constexpr T DIFFUSE_REFLECTION   = 0.01718;
    static constexpr T DIFFUSE_TRANSMISSION = 0.0000541;

    static constexpr int photons = 1e6;
};

/*********
 * TESTS *
 *********/

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_IS_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample1, photons, sample1.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_IS_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample1, photons, multi_thread, sample1.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_GREATER_THAN_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample2, photons, sample2.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_GREATER_THAN_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample2, photons, multi_thread, sample2.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_LESS_THAN_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample3, photons, sample3.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 8e-6);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 8e-6);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionOnly_MFP_LESS_THAN_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample3, photons, multi_thread, sample3.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 8e-6);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 8e-6);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_IS_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample1, photons, sample1.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_IS_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample1, photons, multi_thread, sample1.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 1.5e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_GREATER_THAN_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample2, photons, sample2.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 1.5e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_GREATER_THAN_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample2, photons, multi_thread, sample2.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_LESS_THAN_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample3, photons, sample3.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 8e-6);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 8e-6);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionOnly_MFP_LESS_THAN_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample3, photons, multi_thread, sample3.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 8e-6);
    EXPECT_NEAR(res.diffuseTransmission, BUGER, 8e-6);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_IS_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample1, photons, sample1.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_IS_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample1, photons, multi_thread, sample1.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_GREATER_THAN_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample2, photons, sample2.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_GREATER_THAN_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample2, photons, multi_thread, sample2.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_LESS_THAN_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample3, photons, sample3.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 2e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-5);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_SingleLayerAbsorptionScattering_MFP_LESS_THAN_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample3, photons, multi_thread, sample3.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 2e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-5);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_IS_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample1, photons, sample1.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 5e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_IS_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample1, photons, multi_thread, sample1.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 5e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 2e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_GREATER_THAN_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample2, photons, sample2.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_GREATER_THAN_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample2, photons, multi_thread, sample2.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_LESS_THAN_D, SingleThread) {
    MonteCarlo<T, Nz, Nr> mc(sample3, photons, sample3.getTotalThickness(), selectedRadius);
    MCresults<T, Nz, Nr> res;

    mc.Calculate(res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 6e-6);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0,  TOTAL_TOLERANCE);
}

TEST_P(MC_MultiLayerAbsorptionScattering_MFP_LESS_THAN_D, MultiThread) {
    MCresults<T, Nz, Nr> res;

    MCmultithread<T, Nz, Nr>(sample3, photons, multi_thread, sample3.getTotalThickness(), selectedRadius, res);
    EXPECT_NEAR(res.specularReflection, SPECULAR_REFLECTION, 1e-4);
    EXPECT_NEAR(res.diffuseReflection, DIFFUSE_REFLECTION, 1e-3);
    EXPECT_NEAR(res.diffuseTransmission, DIFFUSE_TRANSMISSION, 6e-6);
    EXPECT_NEAR(res.diffuseTransmission + res.diffuseReflection + res.specularReflection + res.absorbed, 1.0, TOTAL_TOLERANCE);
}

/*************
 * INSTANCES *
 *************/

#define TESTING_VALUES ::testing::Values(1, 2, 3, 4, 5)
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
