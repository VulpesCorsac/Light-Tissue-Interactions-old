#pragma once

#include "MediumInterface.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

class MediumInterfaceTests : public ::testing::Test {
protected:
    MediumInterface<float>* medium = new MediumInterface<float>();
};

TEST_F(MediumInterfaceTests, TypeIsUnknown) {
    EXPECT_EQ(medium->type, MediumType::Unknown);
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
