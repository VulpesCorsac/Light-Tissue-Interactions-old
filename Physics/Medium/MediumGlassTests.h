#pragma once

#include "MediumGlass.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

class MediumGlassTests : public ::testing::Test {
protected:
    MediumGlass<float>* medium = new MediumGlass<float>();
};

TEST_F(MediumGlassTests, TypeIsUnknown) {
    EXPECT_EQ(medium->type, MediumType::Glass);
}

TEST_F(MediumGlassTests, ThrowsExceptionForRefraction) {
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
