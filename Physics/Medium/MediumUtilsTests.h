#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "MediumUtils.h"

#include "../../Utils/Utils.h"
#include "../../Utils/StringUtils.h"

#include <gtest/gtest.h>

using namespace Physics_NS;
using namespace Utils_NS;

TEST(MediumUtilsTests, MediumTypeFromPtr_ThrownExceptionForNullptr) {
    EXPECT_THROW(mediumType<float>(nullptr), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_ThrownExceptionForInterface) {
    std::unique_ptr<MediumInterface<float>> medium = std::make_unique<MediumInterface<float>>();
    EXPECT_THROW(mediumType<float>(medium.get()), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromPtr_ThrownExceptionForUnknownType) {
    class MediumDummy: public MediumInterface<float> {};

    auto medium = std::make_unique<MediumDummy>();
    EXPECT_THROW(mediumType<float>(medium.get()), std::invalid_argument);
}

#define TEST_MEDIUM_TYPE_FROM_PTR(TYPE)                           \
TEST(MediumUtilsTests, MediumTypeFromPtr_Medium##TYPE) {          \
    auto medium = std::make_unique<Medium##TYPE<float>>();        \
    EXPECT_EQ(mediumType<float>(medium.get()), MediumType::TYPE); \
}

TEST_MEDIUM_TYPE_FROM_PTR(Glass)
TEST_MEDIUM_TYPE_FROM_PTR(Constant)
TEST_MEDIUM_TYPE_FROM_PTR(Linear)
TEST_MEDIUM_TYPE_FROM_PTR(Arbitrary)

TEST(MediumUtilsTests, MediumTypeFromStr_ThrownExceptionForEmptyStr) {
    EXPECT_THROW(mediumType(""), std::invalid_argument);
}

TEST(MediumUtilsTests, MediumTypeFromStr_ThrownExceptionForUnknownType) {
    EXPECT_THROW(mediumType("InvalidMedium"), std::invalid_argument);
}

#define TEST_MEDIUM_TYPE_FROM_STR(TYPE)                  \
TEST(MediumUtilsTests, MediumTypeFromStr_Medium##TYPE) { \
    auto type = MediumTypeStrings::TYPE;                 \
    auto types = getAllVariants(to_lower(type));         \
    auto upper = getAllVariants(to_upper(type));         \
    types.insert(ALL_CONTAINER(upper));                  \
    for (const auto& item: types)                        \
        EXPECT_EQ(mediumType(item), MediumType::TYPE);   \
}

TEST_MEDIUM_TYPE_FROM_STR(Glass)
TEST_MEDIUM_TYPE_FROM_STR(Constant)
TEST_MEDIUM_TYPE_FROM_STR(Linear)
TEST_MEDIUM_TYPE_FROM_STR(Arbitrary)

#define TEST_MEDIUM_TYPE_TO_STR(TYPE)                  \
TEST(MediumUtilsTests, MediumTypeToStr_Medium##TYPE) { \
    EXPECT_EQ(to_string(MediumType::TYPE),             \
              MediumTypeStrings::TYPE);                \
}

TEST_MEDIUM_TYPE_TO_STR(Glass)
TEST_MEDIUM_TYPE_TO_STR(Constant)
TEST_MEDIUM_TYPE_TO_STR(Linear)
TEST_MEDIUM_TYPE_TO_STR(Arbitrary)

#define TEST_MEDIUM_TYPE_TO_STR_FROM_STR(TYPE)                \
TEST(MediumUtilsTests, MediumTypeToStrFromStr_Medium##TYPE) { \
    EXPECT_EQ(mediumType(to_string(MediumType::TYPE)),        \
              MediumType::TYPE);                              \
}

TEST_MEDIUM_TYPE_TO_STR_FROM_STR(Glass)
TEST_MEDIUM_TYPE_TO_STR_FROM_STR(Constant)
TEST_MEDIUM_TYPE_TO_STR_FROM_STR(Linear)
TEST_MEDIUM_TYPE_TO_STR_FROM_STR(Arbitrary)

TEST(MediumUtilsTests, Validate_ThrownExceptionForUnknownType) {
    MediumProperties<int> properties;
    properties.type = MediumType::Unknown;
    EXPECT_THROW(validate(properties), std::invalid_argument);
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForUnknownType) {
    MediumProperties<int> properties;
    properties.type = MediumType::Unknown;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.n0 = 1;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.n0 = 1;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.n0 = 1;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.n0 = 1;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.n0 = 1;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.nT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.nD = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.n0 = 1;
    EXPECT_FALSE(validateSafe(properties));
    properties.nT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.nD = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.n0 = 1;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.nT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.nD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.nDT = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithoutRefractionIndex) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.n0 = 1;
    EXPECT_FALSE(validateSafe(properties));
    properties.nT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.nD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.nDT = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.n0 = 1;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.n0 = 1;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 0;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.n0 = 1;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 0;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.n0 = 1;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 0;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.n0 = 1;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 0;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.n0 = 1;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 0;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.n0 = 1;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithRefractionIndexLessThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 0;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.n0 = 1;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithoutDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.r0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithoutDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.r0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.r0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.r0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithoutDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.r0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.rT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.rD = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithoutDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.r0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.rT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.rD = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithoutDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.r0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.rT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.rD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.rDT = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithoutDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.r0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.rT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.rD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.rDT = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithNegativeDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = -1;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.r0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithNegativeDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = -1;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.r0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithNegativeDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = -1;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.r0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithNegativeDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = -1;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.r0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithNegativeDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = -1;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.r0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithNegativeDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = -1;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.r0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithNegativeDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = -1;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.r0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithNegativeDensity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = -1;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.r0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithoutHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.c0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithoutHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.c0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.c0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.c0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithoutHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.c0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.cT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.cD = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithoutHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.c0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.cT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.cD = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithoutHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.c0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.cT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.cD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.cDT = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithoutHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.c0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.cD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.cT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.cDT = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithNegativeHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = 0;
    properties.c0 = -1;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.c0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithNegativeHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = 0;
    properties.c0 = -1;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.c0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithNegativeHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = -1;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.c0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithNegativeHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = -1;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.c0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithNegativeHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = -1;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.c0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithNegativeHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = -1;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.c0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithNegativeHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = -1;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.c0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithNegativeHeatCapacity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = -1;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.c0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithoutThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = 0;
    properties.c0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.k0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithoutThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = 0;
    properties.c0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.k0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.k0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.k0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithoutThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.kT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.kD = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithoutThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.kT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.kD = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithoutThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.kT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.kDT = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithoutThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.kT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.kDT = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForGlassWithNegativeThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = -1;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.k0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForGlassWithNegativeThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 1;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = -1;
    EXPECT_FALSE(validateSafe(properties));
    properties.k0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithNegativeThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = -1;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.k0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithNegativeThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = -1;
    EXPECT_FALSE(validateSafe(properties));
    properties.k0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithNegativeThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = -1;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.k0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithNegativeThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = -1;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.k0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithNegativeThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = -1;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.k0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithNegativeThermalConductivity) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = -1;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.k0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_NoExceptionForProperGlass) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 2;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsTrueForProperGlass) {
    MediumProperties<int> properties;
    properties.type = MediumType::Glass;
    properties.n0 = 2;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutAbsorptionCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.a0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutAbsorptionCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.a0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithoutAbsorptionCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.a0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.aT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.aD = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithoutAbsorptionCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.a0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.aT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.aD = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithoutAbsorptionCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.a0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.aT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.aD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.aDT = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithoutAbsorptionCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.a0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.aT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.aD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.aDT = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithAbsorptionCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = -1;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.a0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithAbsorptionCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = -1;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.a0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithAbsorptionCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = -1;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.a0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithAbsorptionCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = -1;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.a0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithAbsorptionCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = -1;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.a0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithAbsorptionCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = -1;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.a0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutScatteringCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.u0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutScatteringCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.u0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithoutScatteringCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.u0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.uT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.uD = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithoutScatteringCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.u0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.uT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.uD = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithoutScatteringCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.u0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.uT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.uD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.uDT = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithoutScatteringCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.u0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.uT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.uD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.uDT = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithScatteringCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = -1;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.u0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithScatteringCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = -1;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.u0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithScatteringCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = -1;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.u0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithScatteringCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = -1;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.u0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithScatteringCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = -1;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.u0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithScatteringCoefficientLessThanZero) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = -1;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.u0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithoutScatteringAnisotropyCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.g0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithoutScatteringAnisotropyCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.g0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithoutScatteringAnisotropyCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.g0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.gT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.gD = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithoutScatteringAnisotropyCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.g0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.gT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.gD = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithoutScatteringAnisotropyCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.g0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.gT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.gD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.gDT = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithoutScatteringAnisotropyCoefficient) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.g0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.gT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.gD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.gDT = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithScatteringAnisotropyCoefficientLessThanMinusOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = -2;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.g0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithScatteringAnisotropyCoefficientLessThanMinusOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = -2;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.g0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithScatteringAnisotropyCoefficientLessThanMinusOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = -2;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.g0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithScatteringAnisotropyCoefficientLessThanMinusOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = -2;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.g0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithScatteringAnisotropyCoefficientLessThanMinusOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = -2;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.g0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithScatteringAnisotropyCoefficientLessThanMinusOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = -2;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.g0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForConstantMediumWithScatteringAnisotropyCoefficientGreaterThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = +2;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.g0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForConstantMediumWithScatteringAnisotropyCoefficientGreaterThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = +2;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.g0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForLinearMediumWithScatteringAnisotropyCoefficientGreaterThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = +2;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.g0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForLinearMediumWithScatteringAnisotropyCoefficientGreaterThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = +2;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.g0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_ThrownExceptionForArbitraryMediumWithScatteringAnisotropyCoefficientGreaterThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = +2;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_THROW(validate(properties), std::invalid_argument);
    properties.g0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsFalseForArbitraryMediumWithScatteringAnisotropyCoefficientGreaterThanOne) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = +2;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_FALSE(validateSafe(properties));
    properties.g0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_NoExceptionForProperConstantMedium) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsTrueForProperConstantMedium) {
    MediumProperties<int> properties;
    properties.type = MediumType::Constant;
    properties.n0 = 1;
    properties.a0 = 0;
    properties.u0 = 0;
    properties.g0 = 0;
    properties.r0 = 0;
    properties.c0 = 0;
    properties.k0 = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_NoExceptionForProperLinearMedium) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsTrueForProperLinearMedium) {
    MediumProperties<int> properties;
    properties.type = MediumType::Linear;
    properties.n0 = 1;
    properties.nT = properties.nD = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, Validate_NoExceptionForProperArbitraryMedium) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(MediumUtilsTests, ValidateSafe_ReturnsTrueForProperArbitraryMedium) {
    MediumProperties<int> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0 = 1;
    properties.nT = properties.nD = properties.nDT = 0;
    properties.a0 = 0;
    properties.aT = properties.aD = properties.aDT = 0;
    properties.u0 = 0;
    properties.uT = properties.uD = properties.uDT = 0;
    properties.g0 = 0;
    properties.gT = properties.gD = properties.gDT = 0;
    properties.r0 = 0;
    properties.rT = properties.rD = properties.rDT = 0;
    properties.c0 = 0;
    properties.cT = properties.cD = properties.cDT = 0;
    properties.k0 = 0;
    properties.kT = properties.kD = properties.kDT = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(MediumUtilsTests, ExportMediumProperties_ForMediumGlass) {
    constexpr float n0 = 2;
    constexpr float r0 = 3;
    constexpr float c0 = 4;
    constexpr float k0 = 5;
    auto medium = std::make_unique<MediumGlass<float>>(n0, r0, c0, k0);
    const auto& properties = exportMediumProperties(medium.get());
    EXPECT_EQ(properties.type, MediumType::Glass);
    EXPECT_FLOAT_EQ(properties.n0.value(), n0);
    EXPECT_FLOAT_EQ(properties.r0.value(), r0);
    EXPECT_FLOAT_EQ(properties.c0.value(), c0);
    EXPECT_FLOAT_EQ(properties.k0.value(), k0);
}

TEST(MediumUtilsTests, ExportMediumProperties_ForMediumConstant) {
    constexpr float n0 = 2;
    constexpr float a0 = 3;
    constexpr float u0 = 4;
    constexpr float g0 = 5;
    constexpr float r0 = 6;
    constexpr float c0 = 7;
    constexpr float k0 = 8;
    auto medium = std::make_unique<MediumConstant<float>>(n0, a0, u0, g0, r0, c0, k0);
    const auto& properties = exportMediumProperties(medium.get());
    EXPECT_EQ(properties.type, MediumType::Constant);
    EXPECT_FLOAT_EQ(properties.n0.value(), n0);
    EXPECT_FLOAT_EQ(properties.a0.value(), a0);
    EXPECT_FLOAT_EQ(properties.u0.value(), u0);
    EXPECT_FLOAT_EQ(properties.g0.value(), g0);
    EXPECT_FLOAT_EQ(properties.r0.value(), r0);
    EXPECT_FLOAT_EQ(properties.c0.value(), c0);
    EXPECT_FLOAT_EQ(properties.k0.value(), k0);
}

TEST(MediumUtilsTests, ExportMediumProperties_ForMediumLinear) {
    constexpr float n0 = 2;
    constexpr float nT = 3;
    constexpr float nD = 4;
    constexpr float a0 = 5;
    constexpr float aT = 6;
    constexpr float aD = 7;
    constexpr float u0 = 8;
    constexpr float uT = 9;
    constexpr float uD = 10;
    constexpr float g0 = 11;
    constexpr float gT = 12;
    constexpr float gD = 13;
    constexpr float r0 = 14;
    constexpr float rT = 15;
    constexpr float rD = 16;
    constexpr float c0 = 17;
    constexpr float cT = 18;
    constexpr float cD = 19;
    constexpr float k0 = 20;
    constexpr float kT = 21;
    constexpr float kD = 22;
    auto medium = std::make_unique<MediumLinear<float>>(n0, nT, nD,
                                                        a0, aT, aD,
                                                        u0, uT, uD,
                                                        g0, gT, gD,
                                                        r0, rT, rD,
                                                        c0, cT, cD,
                                                        k0, kT, kD);
    const auto& properties = exportMediumProperties(medium.get());
    EXPECT_EQ(properties.type, MediumType::Linear);
    EXPECT_FLOAT_EQ(properties.n0.value(), n0);
    EXPECT_FLOAT_EQ(properties.nT.value(), nT);
    EXPECT_FLOAT_EQ(properties.nD.value(), nD);
    EXPECT_FLOAT_EQ(properties.a0.value(), a0);
    EXPECT_FLOAT_EQ(properties.aT.value(), aT);
    EXPECT_FLOAT_EQ(properties.aD.value(), aD);
    EXPECT_FLOAT_EQ(properties.u0.value(), u0);
    EXPECT_FLOAT_EQ(properties.uT.value(), uT);
    EXPECT_FLOAT_EQ(properties.uD.value(), uD);
    EXPECT_FLOAT_EQ(properties.g0.value(), g0);
    EXPECT_FLOAT_EQ(properties.gT.value(), gT);
    EXPECT_FLOAT_EQ(properties.gD.value(), gD);
    EXPECT_FLOAT_EQ(properties.r0.value(), r0);
    EXPECT_FLOAT_EQ(properties.rT.value(), rT);
    EXPECT_FLOAT_EQ(properties.rD.value(), rD);
    EXPECT_FLOAT_EQ(properties.c0.value(), c0);
    EXPECT_FLOAT_EQ(properties.cT.value(), cT);
    EXPECT_FLOAT_EQ(properties.cD.value(), cD);
    EXPECT_FLOAT_EQ(properties.k0.value(), k0);
    EXPECT_FLOAT_EQ(properties.kT.value(), kT);
    EXPECT_FLOAT_EQ(properties.kD.value(), kD);
}

TEST(MediumUtilsTests, ExportMediumProperties_ForMediumArbitrary) {
    constexpr float n0  = 2;
    constexpr float nT  = 3;
    constexpr float nD  = 4;
    constexpr float nDT = 5;
    constexpr float a0  = 6;
    constexpr float aT  = 7;
    constexpr float aD  = 8;
    constexpr float aDT = 9;
    constexpr float u0  = 10;
    constexpr float uT  = 11;
    constexpr float uD  = 12;
    constexpr float uDT = 13;
    constexpr float g0  = 14;
    constexpr float gT  = 15;
    constexpr float gD  = 16;
    constexpr float gDT = 17;
    constexpr float r0  = 18;
    constexpr float rT  = 19;
    constexpr float rD  = 20;
    constexpr float rDT = 21;
    constexpr float c0  = 22;
    constexpr float cT  = 23;
    constexpr float cD  = 24;
    constexpr float cDT = 25;
    constexpr float k0  = 26;
    constexpr float kT  = 27;
    constexpr float kD  = 28;
    constexpr float kDT = 29;
    auto medium = std::make_unique<MediumArbitrary<float>>(n0, nT, nD, nDT,
                                                           a0, aT, aD, aDT,
                                                           u0, uT, uD, uDT,
                                                           g0, gT, gD, gDT,
                                                           r0, rT, rD, rDT,
                                                           c0, cT, cD, cDT,
                                                           k0, kT, kD, kDT);
    const auto& properties = exportMediumProperties(medium.get());
    EXPECT_EQ(properties.type, MediumType::Arbitrary);
    EXPECT_FLOAT_EQ(properties.n0.value() , n0 );
    EXPECT_FLOAT_EQ(properties.nT.value() , nT );
    EXPECT_FLOAT_EQ(properties.nD.value() , nD );
    EXPECT_FLOAT_EQ(properties.nDT.value(), nDT);
    EXPECT_FLOAT_EQ(properties.a0.value() , a0 );
    EXPECT_FLOAT_EQ(properties.aT.value() , aT );
    EXPECT_FLOAT_EQ(properties.aD.value() , aD );
    EXPECT_FLOAT_EQ(properties.aDT.value(), aDT);
    EXPECT_FLOAT_EQ(properties.u0.value() , u0 );
    EXPECT_FLOAT_EQ(properties.uT.value() , uT );
    EXPECT_FLOAT_EQ(properties.uD.value() , uD );
    EXPECT_FLOAT_EQ(properties.uDT.value(), uDT);
    EXPECT_FLOAT_EQ(properties.g0.value() , g0 );
    EXPECT_FLOAT_EQ(properties.gT.value() , gT );
    EXPECT_FLOAT_EQ(properties.gD.value() , gD );
    EXPECT_FLOAT_EQ(properties.gDT.value(), gDT);
    EXPECT_FLOAT_EQ(properties.r0.value() , r0 );
    EXPECT_FLOAT_EQ(properties.rT.value() , rT );
    EXPECT_FLOAT_EQ(properties.rD.value() , rD );
    EXPECT_FLOAT_EQ(properties.rDT.value(), rDT);
    EXPECT_FLOAT_EQ(properties.c0.value() , c0 );
    EXPECT_FLOAT_EQ(properties.cT.value() , cT );
    EXPECT_FLOAT_EQ(properties.cD.value() , cD );
    EXPECT_FLOAT_EQ(properties.cDT.value(), cDT);
    EXPECT_FLOAT_EQ(properties.k0.value() , k0 );
    EXPECT_FLOAT_EQ(properties.kT.value() , kT );
    EXPECT_FLOAT_EQ(properties.kD.value() , kD );
    EXPECT_FLOAT_EQ(properties.kDT.value(), kDT);
}
