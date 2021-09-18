#pragma once

#include "MediumGlass.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

class MediumGlassTests : public ::testing::Test {
protected:
    MediumGlass<float>* const medium = new MediumGlass<float>();
};

TEST_F(MediumGlassTests, ConstructorFromRefractionIndex) {
    constexpr float refractionIndex = 2;
    MediumGlass<float>* const medium = new MediumGlass<float>(refractionIndex);
    EXPECT_EQ(medium->refraction(), refractionIndex);
}

TEST_F(MediumGlassTests, ConstructorFromMediumProperties) {
    constexpr float refractionIndex = 2;
    MediumProperties<float> properties;
    properties.n0 = refractionIndex;
    MediumGlass<float>* const medium = new MediumGlass<float>(properties);
    EXPECT_EQ(medium->refraction(), refractionIndex);
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
