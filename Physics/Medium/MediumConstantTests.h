#pragma once

#include "MediumConstant.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;

class MediumConstantTests : public ::testing::Test {
protected:
    std::unique_ptr<MediumConstant<float>> medium = std::make_unique<MediumConstant<float>>();

    static constexpr float n0 = 2;
    static constexpr float a0 = 3;
    static constexpr float u0 = 4;
    static constexpr float g0 = 5;
    std::unique_ptr<MediumConstant<float>> nondefaultMedium = std::make_unique<MediumConstant<float>>(n0,
                                                                                                      a0,
                                                                                                      u0,
                                                                                                      g0);
};

TEST_F(MediumConstantTests, TypeIsConstant) {
    EXPECT_EQ(medium->type, MediumType::Constant);
}

TEST_F(MediumConstantTests, ConstructorFromParams) {
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction(), n0);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption(), a0);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering(), u0);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy(), g0);
}

TEST_F(MediumConstantTests, ConstructorFromMediumProperties) {
    MediumProperties<float> properties;
    properties.n0 = n0;
    properties.a0 = a0;
    properties.u0 = u0;
    properties.g0 = g0;
    auto generatedMedium = std::make_unique<MediumConstant<float>>(properties);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(), n0);
    EXPECT_FLOAT_EQ(generatedMedium->absorption(), a0);
    EXPECT_FLOAT_EQ(generatedMedium->scattering(), u0);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy(), g0);
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

TEST_F(MediumConstantTests, GetInteraction) {
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(), nondefaultMedium->absorption() + nondefaultMedium->scattering());
}

TEST_F(MediumConstantTests, GetAlbedo) {
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(), nondefaultMedium->scattering() / nondefaultMedium->interaction());
}
