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
    std::unique_ptr<MediumInterface<float>> medium = std::make_unique<MediumInterface<float>>();
    EXPECT_THROW(mediumType<float>(medium.get()), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_ThrownExceptionForUnknownType) {
    class MediunDummy: public MediumInterface<float> {};

    auto medium = std::make_unique<MediunDummy>();
    EXPECT_THROW(mediumType<float>(medium.get()), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_MediumGlass) {
    auto medium = std::make_unique<MediumGlass<float>>();
    EXPECT_EQ(mediumType<float>(medium.get()), MediumType::Glass);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_MediumConstant) {
    auto medium = std::make_unique<MediumConstant<float>>();
    EXPECT_EQ(mediumType<float>(medium.get()), MediumType::Constant);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_MediumLinear) {
    auto medium = std::make_unique<MediumLinear<float>>();
    EXPECT_EQ(mediumType<float>(medium.get()), MediumType::Linear);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_MediumArbitrary) {
    auto medium = std::make_unique<MediumArbitrary<float>>();
    EXPECT_EQ(mediumType<float>(medium.get()), MediumType::Arbitrary);
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

TEST(MediumUtilsTests, MediumTypeFromStr_MediumArbitrary) {
    EXPECT_EQ(mediumType("arbitrary"), MediumType::Arbitrary);
    EXPECT_EQ(mediumType("Arbitrary"), MediumType::Arbitrary);
    EXPECT_EQ(mediumType("ARBITRARY"), MediumType::Arbitrary);
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForUnknownType) {
    MediumProperties<int> properties;
    properties.type = MediumType::Unknown;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForUnknownType) {
    MediumProperties<int> properties;
    properties.type = MediumType::Unknown;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 0;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 0;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_NoExceptionForProperGlass) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 2;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsTrueForProperGlass) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 2;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 0;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 0;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutAbsorptionCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutAbsorptionCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithAbsorptionCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = -1;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithAbsorptionCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = -1;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutScatteringCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.g0 = 0;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutScatteringCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.g0 = 0;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithScatteringCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = -1;
    properties.g0 = 0;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithScatteringCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = -1;
    properties.g0 = 0;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutScatteringAnisotropyCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutScatteringAnisotropyCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithScatteringAnisotropyCoefficientLessThanMinusOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = -2;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithScatteringAnisotropyCoefficientLessThanMinusOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = -2;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithScatteringAnisotropyCoefficientGreaterThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = +2;
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithScatteringAnisotropyCoefficientGreaterThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = +2;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_NoExceptionForProperConstantMedium) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsTrueForProperConstantMedium) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}
