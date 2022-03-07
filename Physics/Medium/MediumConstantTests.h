#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "MediumConstant.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;
using namespace std;

class MediumConstantTests : public ::testing::Test {
protected:
    unique_ptr<MediumConstant<float>> medium = make_unique<MediumConstant<float>>();

    static constexpr float n0 = 2;
    static constexpr float a0 = 3;
    static constexpr float u0 = 4;
    static constexpr float g0 = 5;
    static constexpr float r0 = 6;
    static constexpr float c0 = 7;
    static constexpr float k0 = 8;
    unique_ptr<MediumConstant<float>> nondefaultMedium = make_unique<MediumConstant<float>>(n0,
                                                                                            a0,
                                                                                            u0,
                                                                                            g0,
                                                                                            r0,
                                                                                            c0,
                                                                                            k0);
};

TEST_F(MediumConstantTests, TypeIsConstant) {
    EXPECT_EQ(medium->type, MediumType::Constant);
}

TEST_F(MediumConstantTests, ConstructorFromParams) {
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction()          , n0);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption()          , a0);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering()          , u0);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy()          , g0);
    EXPECT_FLOAT_EQ(nondefaultMedium->density()             , r0);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity()       , c0);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(), k0);
}

TEST_F(MediumConstantTests, ConstructorFromMediumProperties) {
    MediumProperties<float> properties;
    properties.n0 = n0;
    properties.a0 = a0;
    properties.u0 = u0;
    properties.g0 = g0;
    properties.r0 = r0;
    properties.c0 = c0;
    properties.k0 = k0;
    auto generatedMedium = make_unique<MediumConstant<float>>(properties);
    EXPECT_FLOAT_EQ(generatedMedium->refraction()          , n0);
    EXPECT_FLOAT_EQ(generatedMedium->absorption()          , a0);
    EXPECT_FLOAT_EQ(generatedMedium->scattering()          , u0);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy()          , g0);
    EXPECT_FLOAT_EQ(generatedMedium->density()             , r0);
    EXPECT_FLOAT_EQ(generatedMedium->heat_capacity()       , c0);
    EXPECT_FLOAT_EQ(generatedMedium->thermal_conductivity(), k0);
}

TEST_F(MediumConstantTests, GetDefaultRefraction) {
    EXPECT_FLOAT_EQ(medium->refraction(), 1);
}

TEST_F(MediumConstantTests, GetDefaultAbsorption) {
    EXPECT_FLOAT_EQ(medium->absorption(), 0);
}

TEST_F(MediumConstantTests, GetDefaultScattering) {
    EXPECT_FLOAT_EQ(medium->scattering(), 0);
}

TEST_F(MediumConstantTests, GetDefaultScatteringAnisotropy) {
    EXPECT_FLOAT_EQ(medium->anisotropy(), 0);
}

TEST_F(MediumConstantTests, GetDefaultDensity) {
    EXPECT_FLOAT_EQ(medium->density(), 0);
}

TEST_F(MediumConstantTests, GetDefaultHeatCapacity) {
    EXPECT_FLOAT_EQ(medium->heat_capacity(), 0);
}

TEST_F(MediumConstantTests, GetDefaultThermalConductivity) {
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(), 0);
}

TEST_F(MediumConstantTests, GetInteraction) {
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(), nondefaultMedium->absorption() + nondefaultMedium->scattering());
}

TEST_F(MediumConstantTests, GetAlbedo) {
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(), nondefaultMedium->scattering() / nondefaultMedium->interaction());
}

TEST_F(MediumConstantTests, GetThermalDiffusivity) {
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(), nondefaultMedium->thermal_conductivity() / nondefaultMedium->density() / nondefaultMedium->heat_capacity());
}
