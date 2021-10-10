#pragma once

#include "MediumGlass.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;

class MediumGlassTests : public ::testing::Test {
protected:
    std::unique_ptr<MediumGlass<float>> medium = std::make_unique<MediumGlass<float>>();

    static constexpr float n0 = 2;
    std::unique_ptr<MediumGlass<float>> nondefaultMedium = std::make_unique<MediumGlass<float>>(n0);
};

TEST_F(MediumGlassTests, ConstructorFromRefractionIndex) {
    EXPECT_EQ(nondefaultMedium->refraction(), n0);
}

TEST_F(MediumGlassTests, ConstructorFromMediumProperties) {
    MediumProperties<float> properties;
    properties.n0 = n0;
    auto generatedMedium = std::make_unique<MediumGlass<float>>(properties);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(), n0);
}

TEST_F(MediumGlassTests, TypeIsGlass) {
    EXPECT_EQ(medium->type, MediumType::Glass);
}

TEST_F(MediumGlassTests, GetDefaultRefraction) {
    EXPECT_FLOAT_EQ(medium->refraction(), 1);
}

TEST_F(MediumGlassTests, ThrowsExceptionForAbsorption) {
    EXPECT_THROW(medium->absorption(), std::runtime_error);
}

TEST_F(MediumGlassTests, ThrowsExceptionForScattering) {
    EXPECT_THROW(medium->scattering(), std::runtime_error);
}

TEST_F(MediumGlassTests, ThrowsExceptionForAnisotropy) {
    EXPECT_THROW(medium->anisotropy(), std::runtime_error);
}

TEST_F(MediumGlassTests, ThrowsExceptionForInteraction) {
    EXPECT_THROW(medium->interaction(), std::runtime_error);
}

TEST_F(MediumGlassTests, ThrowsExceptionForAlbedo) {
    EXPECT_THROW(medium->albedo(), std::runtime_error);
}
