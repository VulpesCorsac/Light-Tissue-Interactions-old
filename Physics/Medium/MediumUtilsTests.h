#pragma once

#include "MediumUtils.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

TEST(MediumUtilsTests, MediumType_ThrownExceptionForNullptr) {
    EXPECT_THROW(mediumType<float>(nullptr), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumType_ThrownExceptionForInterface) {
    auto* medium = new MediumInterface<float>();
    EXPECT_THROW(mediumType<float>(medium), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumType_ThrownExceptionForUnknownType) {
    class MediunDummy: public MediumInterface<float> {};

    auto* medium = new MediunDummy();
    EXPECT_THROW(mediumType<float>(medium), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumType_MediumGlass) {
    auto* medium = new MediumGlass<float>();
    EXPECT_EQ(mediumType<float>(medium), MediumType::Glass);
}
