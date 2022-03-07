#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "MediumInterface.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;
using namespace std;

class MediumInterfaceTests : public ::testing::Test {
protected:
    unique_ptr<MediumInterface<float>> medium = make_unique<MediumInterface<float>>();
};

TEST_F(MediumInterfaceTests, TypeIsUnknown) {
    EXPECT_EQ(medium->type, MediumType::Unknown);
}

TEST_F(MediumInterfaceTests, Constructor) {
    unique_ptr<MediumInterface<float>> generatedMedium = make_unique<MediumInterface<float>>(MediumType::Glass);
    EXPECT_EQ(generatedMedium->type, MediumType::Glass);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForRefraction) {
    EXPECT_THROW(medium->refraction(), runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForAbsorption) {
    EXPECT_THROW(medium->absorption(), runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForScattering) {
    EXPECT_THROW(medium->scattering(), runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForAnisotropy) {
    EXPECT_THROW(medium->anisotropy(), runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForInteraction) {
    EXPECT_THROW(medium->interaction(), runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForAlbedo) {
    EXPECT_THROW(medium->albedo(), runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForDensity) {
    EXPECT_THROW(medium->density(), runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForHeatCapacity) {
    EXPECT_THROW(medium->heat_capacity(), runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForThermalConductivity) {
    EXPECT_THROW(medium->thermal_conductivity(), runtime_error);
}

TEST_F(MediumInterfaceTests, ThrowsExceptionForThermalDiffusivity) {
    EXPECT_THROW(medium->thermal_diffusivity(), runtime_error);
}
