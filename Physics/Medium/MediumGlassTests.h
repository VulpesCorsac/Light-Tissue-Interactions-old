#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "MediumGlass.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;
using namespace std;

class MediumGlassTests : public ::testing::Test {
protected:
    unique_ptr<MediumGlass<float>> medium = make_unique<MediumGlass<float>>();

    static constexpr float n0 = 2;
    static constexpr float r0 = 3;
    static constexpr float c0 = 4;
    static constexpr float k0 = 5;
    unique_ptr<MediumGlass<float>> nondefaultMedium = make_unique<MediumGlass<float>>(n0, r0, c0, k0);
};

TEST_F(MediumGlassTests, ConstructorFromParams) {
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction()          , n0);
    EXPECT_FLOAT_EQ(nondefaultMedium->density()             , r0);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity()       , c0);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(), k0);
}

TEST_F(MediumGlassTests, ConstructorFromMediumProperties) {
    MediumProperties<float> properties;
    properties.n0 = n0;
    properties.r0 = r0;
    properties.c0 = c0;
    properties.k0 = k0;
    auto generatedMedium = make_unique<MediumGlass<float>>(properties);
    EXPECT_FLOAT_EQ(generatedMedium->refraction()          , n0);
    EXPECT_FLOAT_EQ(generatedMedium->density()             , r0);
    EXPECT_FLOAT_EQ(generatedMedium->heat_capacity()       , c0);
    EXPECT_FLOAT_EQ(generatedMedium->thermal_conductivity(), k0);
}

TEST_F(MediumGlassTests, TypeIsGlass) {
    EXPECT_EQ(medium->type, MediumType::Glass);
}

TEST_F(MediumGlassTests, GetDefaultRefraction) {
    EXPECT_FLOAT_EQ(medium->refraction(), 1);
}

TEST_F(MediumGlassTests, GetDefaultDensity) {
    EXPECT_FLOAT_EQ(medium->density(), 0);
}

TEST_F(MediumGlassTests, GetDefaultHeatCapacity) {
    EXPECT_FLOAT_EQ(medium->heat_capacity(), 0);
}

TEST_F(MediumGlassTests, GetDefaultThermalConductivity) {
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(), 0);
}

TEST_F(MediumGlassTests, ThrowsExceptionForAbsorption) {
    EXPECT_THROW(medium->absorption(), runtime_error);
}

TEST_F(MediumGlassTests, ThrowsExceptionForScattering) {
    EXPECT_THROW(medium->scattering(), runtime_error);
}

TEST_F(MediumGlassTests, ThrowsExceptionForAnisotropy) {
    EXPECT_THROW(medium->anisotropy(), runtime_error);
}

TEST_F(MediumGlassTests, ThrowsExceptionForInteraction) {
    EXPECT_THROW(medium->interaction(), runtime_error);
}

TEST_F(MediumGlassTests, ThrowsExceptionForAlbedo) {
    EXPECT_THROW(medium->albedo(), runtime_error);
}

TEST_F(MediumGlassTests, GetThermalDiffusivity) {
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(),
                    nondefaultMedium->thermal_conductivity() / nondefaultMedium->density() / nondefaultMedium->heat_capacity());
}
