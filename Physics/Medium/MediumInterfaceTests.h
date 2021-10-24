#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "MediumInterface.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;

class MediumInterfaceTests : public ::testing::Test {
protected:
    std::unique_ptr<MediumInterface<float>> medium = std::make_unique<MediumInterface<float>>();
};

TEST_F(MediumInterfaceTests, TypeIsUnknown) {
    EXPECT_EQ(medium->type, MediumType::Unknown);
}

TEST_F(MediumInterfaceTests, Constructor) {
    std::unique_ptr<MediumInterface<float>> generatedMedium = std::make_unique<MediumInterface<float>>(MediumType::Glass);
    EXPECT_EQ(generatedMedium->type, MediumType::Glass);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForRefraction) {
    EXPECT_THROW(medium->refraction(), std::runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForAbsorption) {
    EXPECT_THROW(medium->absorption(), std::runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForScattering) {
    EXPECT_THROW(medium->scattering(), std::runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForAnisotropy) {
    EXPECT_THROW(medium->anisotropy(), std::runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForInteraction) {
    EXPECT_THROW(medium->interaction(), std::runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForAlbedo) {
    EXPECT_THROW(medium->albedo(), std::runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForDensity) {
    EXPECT_THROW(medium->density(), std::runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForHeatCapacity) {
    EXPECT_THROW(medium->heat_capacity(), std::runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForThermalConductivity) {
    EXPECT_THROW(medium->thermal_conductivity(), std::runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForThermalDiffusivity) {
    EXPECT_THROW(medium->thermal_diffusivity(), std::runtime_error);
}
