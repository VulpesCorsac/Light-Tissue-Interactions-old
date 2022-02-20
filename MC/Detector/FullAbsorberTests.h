#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "FullAbsorber.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;

class FullAbsorberTests : public ::testing::Test {
protected:
    std::unique_ptr<FullAbsorber<float>> detector = std::make_unique<FullAbsorber<float>>();

    static constexpr float collimatedCosine = 0.9;
    std::unique_ptr<FullAbsorber<float>> nondefaultDetector = std::make_unique<FullAbsorber<float>>(collimatedCosine);
};

TEST_F(FullAbsorberTests, InitialAbsorbedIsZero) {
    EXPECT_FLOAT_EQ(detector->collimatedAbsorbed, 0);
    EXPECT_FLOAT_EQ(detector->diffusiveAbsorbed, 0);
}

TEST_F(FullAbsorberTests, ConstructorFromCollimatedCosine) {
    EXPECT_FLOAT_EQ(nondefaultDetector->collimatedCosine, collimatedCosine);
}

TEST_F(FullAbsorberTests, ConstructorFromMediumProperties) {
    DetectorProperties<float> properties;
    properties.collimatedCosine = collimatedCosine;
    auto generated_detector = std::make_unique<FullAbsorber<float>>(properties);
    EXPECT_FLOAT_EQ(generated_detector->collimatedCosine, collimatedCosine);
}

TEST_F(FullAbsorberTests, TypeIsFullAbsorber) {
    EXPECT_EQ(detector->type, DetectorType::FullAbsorber);
}

TEST_F(FullAbsorberTests, DetectCollimated) {
    Photon<float> photon;
    photon.direction.z = 1;
    photon.weight = 1;
    nondefaultDetector->detect(photon);
    EXPECT_FLOAT_EQ(nondefaultDetector->collimatedAbsorbed, photon.weight);
    EXPECT_FLOAT_EQ(nondefaultDetector->diffusiveAbsorbed , 0            );
}

TEST_F(FullAbsorberTests, DetectDiffusive) {
    Photon<float> photon;
    photon.direction.z = 0.5;
    photon.weight = 0.5;
    nondefaultDetector->detect(photon);
    EXPECT_FLOAT_EQ(nondefaultDetector->diffusiveAbsorbed , photon.weight);
    EXPECT_FLOAT_EQ(nondefaultDetector->collimatedAbsorbed, 0            );
}

TEST_F(FullAbsorberTests, Calibrate) {
    nondefaultDetector->collimatedAbsorbed = 100;
    nondefaultDetector->diffusiveAbsorbed  = 100;
    nondefaultDetector->calibrate(1000);
    EXPECT_FLOAT_EQ(nondefaultDetector->collimatedAbsorbed, 0.1);
    EXPECT_FLOAT_EQ(nondefaultDetector->diffusiveAbsorbed , 0.1);
}
