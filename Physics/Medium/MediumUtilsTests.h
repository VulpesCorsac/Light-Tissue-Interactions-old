#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "MediumUtils.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

TEST(MediumUtilsTests, MediumTypeFromPtr_ThrownExceptionForNullptr) {
    EXPECT_THROW(mediumType<float>(nullptr), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_ThrownExceptionForInterface) {
    auto* const medium = new MediumInterface<float>();
    EXPECT_THROW(mediumType<float>(medium), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_ThrownExceptionForUnknownType) {
    class MediunDummy: public MediumInterface<float> {};

    auto* const medium = new MediunDummy();
    EXPECT_THROW(mediumType<float>(medium), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_MediumGlass) {
    auto* const medium = new MediumGlass<float>();
    EXPECT_EQ(mediumType<float>(medium), MediumType::Glass);
}

TEST(MediumUtilsTests, MediumTypeFromStr_ThrownExceptionForEmptyStr) {
    EXPECT_THROW(mediumType(""), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromStr_ThrownExceptionForUnknownType) {
    EXPECT_THROW(mediumType("InvalidMedium"), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromStr_MediumGlass) {
    EXPECT_EQ(mediumType("glass"), MediumType::Glass);
    EXPECT_EQ(mediumType("Glass"), MediumType::Glass);
    EXPECT_EQ(mediumType("GLASS"), MediumType::Glass);
}
