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

TEST(MediumUtilsTests, MediumTypeFromPtr_MediumConstant) {
    auto* const medium = new MediumConstant<float>();
    EXPECT_EQ(mediumType<float>(medium), MediumType::Constant);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_MediumLinear) {
    auto* const medium = new MediumLinear<float>();
    EXPECT_EQ(mediumType<float>(medium), MediumType::Linear);
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

TEST(MediumUtilsTests, MediumTypeFromStr_MediumConstant) {
    EXPECT_EQ(mediumType("constant"), MediumType::Constant);
    EXPECT_EQ(mediumType("Constant"), MediumType::Constant);
    EXPECT_EQ(mediumType("CONSTANT"), MediumType::Constant);
}

TEST(MediumUtilsTests, MediumTypeFromStr_MediumLinear) {
    EXPECT_EQ(mediumType("linear"), MediumType::Linear);
    EXPECT_EQ(mediumType("Linear"), MediumType::Linear);
    EXPECT_EQ(mediumType("LINEAR"), MediumType::Linear);
}
