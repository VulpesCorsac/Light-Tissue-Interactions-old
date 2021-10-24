#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "SettingsExporterMedium.h"
#include "SettingsImporterMedium.h"

#include <gtest/gtest.h>

using namespace Physics_NS;
using namespace Settings_NS;

TEST(SettingsImporterMediumTests, MediumType_EmptyString) {
    EXPECT_THROW(mediumType(YAML::Load("")), std::invalid_argument);
}

TEST(SettingsImporterMediumTests, MediumType_InvalidType) {
    EXPECT_THROW(mediumType(YAML::Load("Kwa")), std::invalid_argument);
    EXPECT_THROW(mediumType(YAML::Load(SettingsStrings::Yaml::Value + ": Kwa")), std::invalid_argument);
}

TEST(SettingsImporterMediumTests, MediumType_Glass) {
    EXPECT_EQ(mediumType(YAML::Load(Physics_NS::MediumTypeStrings::Glass)), Physics_NS::MediumType::Glass);
    EXPECT_EQ(mediumType(YAML::Load(SettingsStrings::Yaml::Value + ": " + Physics_NS::MediumTypeStrings::Glass)), Physics_NS::MediumType::Glass);
}

TEST(SettingsImporterMediumTests, MediumType_Constant) {
    EXPECT_EQ(mediumType(YAML::Load(Physics_NS::MediumTypeStrings::Constant)), Physics_NS::MediumType::Constant);
    EXPECT_EQ(mediumType(YAML::Load(SettingsStrings::Yaml::Value + ": " + Physics_NS::MediumTypeStrings::Constant)), Physics_NS::MediumType::Constant);
}

TEST(SettingsImporterMediumTests, MediumType_Linear) {
    EXPECT_EQ(mediumType(YAML::Load(Physics_NS::MediumTypeStrings::Linear)), Physics_NS::MediumType::Linear);
    EXPECT_EQ(mediumType(YAML::Load(SettingsStrings::Yaml::Value + ": " + Physics_NS::MediumTypeStrings::Linear)), Physics_NS::MediumType::Linear);
}

TEST(SettingsImporterMediumTests, MediumType_Arbitrary) {
    EXPECT_EQ(mediumType(YAML::Load(Physics_NS::MediumTypeStrings::Arbitrary)), Physics_NS::MediumType::Arbitrary);
    EXPECT_EQ(mediumType(YAML::Load(SettingsStrings::Yaml::Value + ": " + Physics_NS::MediumTypeStrings::Arbitrary)), Physics_NS::MediumType::Arbitrary);
}

TEST(SettingsImporterMediumTests, MediumProperties_MediumTypeGlass) {
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Glass)).type, Physics_NS::MediumType::Glass);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Glass)).type, Physics_NS::MediumType::Glass);
}

TEST(SettingsImporterMediumTests, MediumProperties_MediumTypeConstant) {
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Constant)).type, Physics_NS::MediumType::Constant);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Constant)).type, Physics_NS::MediumType::Constant);
}

TEST(SettingsImporterMediumTests, MediumProperties_MediumTypeLinear) {
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Linear)).type, Physics_NS::MediumType::Linear);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Linear)).type, Physics_NS::MediumType::Linear);
}

TEST(SettingsImporterMediumTests, MediumProperties_MediumTypeArbitrary) {
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Arbitrary)).type, Physics_NS::MediumType::Arbitrary);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Arbitrary)).type, Physics_NS::MediumType::Arbitrary);
}

TEST(SettingsImporterMediumTests, MediumProperties_N0) {
    constexpr float n0 = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::N0 + ": " + std::to_string(n0))).n0, n0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::N0 + ": " + std::to_string(n0))).n0, n0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::N0 + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(n0))).n0, n0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::N0 + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(n0))).n0, n0);
}

TEST(SettingsImporterMediumTests, MediumProperties_N0ValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::N0 + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_N0SetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::N0 + ": 1\n" + SettingsStrings::Medium::N0 + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NT) {
    constexpr float nT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::NT + ": " + std::to_string(nT))).nT, nT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::NT + ": " + std::to_string(nT))).nT, nT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::NT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(nT))).nT, nT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::NT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(nT))).nT, nT);
}

TEST(SettingsImporterMediumTests, MediumProperties_NTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::NT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::NT + ": 1\n" + SettingsStrings::Medium::NT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_ND) {
    constexpr float nD = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::ND + ": " + std::to_string(nD))).nD, nD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::ND + ": " + std::to_string(nD))).nD, nD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::ND + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(nD))).nD, nD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::ND + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(nD))).nD, nD);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::ND + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::ND + ": 1\n" + SettingsStrings::Medium::ND + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDT) {
    constexpr float nDT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::NDT + ": " + std::to_string(nDT))).nDT, nDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::NDT + ": " + std::to_string(nDT))).nDT, nDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::NDT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(nDT))).nDT, nDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::NDT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(nDT))).nDT, nDT);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::NDT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::NDT + ": 1\n" + SettingsStrings::Medium::NDT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_A0) {
    constexpr float a0 = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::A0 + ": " + std::to_string(a0))).a0, a0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::A0 + ": " + std::to_string(a0))).a0, a0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::A0 + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(a0))).a0, a0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::A0 + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(a0))).a0, a0);
}

TEST(SettingsImporterMediumTests, MediumProperties_A0ValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::A0 + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_A0SetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::A0 + ": 1\n" + SettingsStrings::Medium::A0 + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_AT) {
    constexpr float aT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::AT + ": " + std::to_string(aT))).aT, aT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::AT + ": " + std::to_string(aT))).aT, aT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::AT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(aT))).aT, aT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::AT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(aT))).aT, aT);
}

TEST(SettingsImporterMediumTests, MediumProperties_ATValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::AT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_ATSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::AT + ": 1\n" + SettingsStrings::Medium::AT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_AD) {
    constexpr float aD = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::AD + ": " + std::to_string(aD))).aD, aD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::AD + ": " + std::to_string(aD))).aD, aD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::AD + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(aD))).aD, aD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::AD + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(aD))).aD, aD);
}

TEST(SettingsImporterMediumTests, MediumProperties_ADValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::AD + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_ADSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::AD + ": 1\n" + SettingsStrings::Medium::AD + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_ADT) {
    constexpr float aDT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::ADT + ": " + std::to_string(aDT))).aDT, aDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::ADT + ": " + std::to_string(aDT))).aDT, aDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::ADT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(aDT))).aDT, aDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::ADT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(aDT))).aDT, aDT);
}

TEST(SettingsImporterMediumTests, MediumProperties_ADTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::ADT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_ADTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::ADT + ": 1\n" + SettingsStrings::Medium::ADT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_U0) {
    constexpr float u0 = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::U0 + ": " + std::to_string(u0))).u0, u0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::U0 + ": " + std::to_string(u0))).u0, u0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::U0 + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(u0))).u0, u0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::U0 + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(u0))).u0, u0);
}

TEST(SettingsImporterMediumTests, MediumProperties_U0ValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::U0 + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_U0SetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::U0 + ": 1\n" + SettingsStrings::Medium::U0 + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_UT) {
    constexpr float uT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UT + ": " + std::to_string(uT))).uT, uT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::UT + ": " + std::to_string(uT))).uT, uT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(uT))).uT, uT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::UT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(uT))).uT, uT);
}

TEST(SettingsImporterMediumTests, MediumProperties_UTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_UTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UT + ": 1\n" + SettingsStrings::Medium::UT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_UD) {
    constexpr float uD = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UD + ": " + std::to_string(uD))).uD, uD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::UD + ": " + std::to_string(uD))).uD, uD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UD + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(uD))).uD, uD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::UD + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(uD))).uD, uD);
}

TEST(SettingsImporterMediumTests, MediumProperties_UDValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UD + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_UDSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UD + ": 1\n" + SettingsStrings::Medium::UD + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_UDT) {
    constexpr float uDT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UDT + ": " + std::to_string(uDT))).uDT, uDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::UDT + ": " + std::to_string(uDT))).uDT, uDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UDT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(uDT))).uDT, uDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::UDT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(uDT))).uDT, uDT);
}

TEST(SettingsImporterMediumTests, MediumProperties_UDTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UDT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_UDTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::UDT + ": 1\n" + SettingsStrings::Medium::UDT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_G0) {
    constexpr float g0 = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::G0 + ": " + std::to_string(g0))).g0, g0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::G0 + ": " + std::to_string(g0))).g0, g0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::G0 + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(g0))).g0, g0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::G0 + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(g0))).g0, g0);
}

TEST(SettingsImporterMediumTests, MediumProperties_G0ValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::G0 + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_G0SetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::G0 + ": 1\n" + SettingsStrings::Medium::G0 + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_GT) {
    constexpr float gT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GT + ": " + std::to_string(gT))).gT, gT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::GT + ": " + std::to_string(gT))).gT, gT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(gT))).gT, gT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::GT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(gT))).gT, gT);
}

TEST(SettingsImporterMediumTests, MediumProperties_GTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_GTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GT + ": 1\n" + SettingsStrings::Medium::GT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_GD) {
    constexpr float gD = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GD + ": " + std::to_string(gD))).gD, gD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::GD + ": " + std::to_string(gD))).gD, gD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GD + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(gD))).gD, gD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::GD + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(gD))).gD, gD);
}

TEST(SettingsImporterMediumTests, MediumProperties_GDValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GD + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_GDSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GD + ": 1\n" + SettingsStrings::Medium::GD + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_GDT) {
    constexpr float gDT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GDT + ": " + std::to_string(gDT))).gDT, gDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::GDT + ": " + std::to_string(gDT))).gDT, gDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GDT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(gDT))).gDT, gDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::GDT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(gDT))).gDT, gDT);
}

TEST(SettingsImporterMediumTests, MediumProperties_GDTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GDT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_GDTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::GDT + ": 1\n" + SettingsStrings::Medium::GDT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_R0) {
    constexpr float r0 = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::R0 + ": " + std::to_string(r0))).r0, r0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::R0 + ": " + std::to_string(r0))).r0, r0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::R0 + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(r0))).r0, r0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::R0 + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(r0))).r0, r0);
}

TEST(SettingsImporterMediumTests, MediumProperties_R0ValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::R0 + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_R0SetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::R0 + ": 1\n" + SettingsStrings::Medium::R0 + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_RT) {
    constexpr float rT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RT + ": " + std::to_string(rT))).rT, rT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::RT + ": " + std::to_string(rT))).rT, rT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(rT))).rT, rT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::RT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(rT))).rT, rT);
}

TEST(SettingsImporterMediumTests, MediumProperties_RTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_RTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RT + ": 1\n" + SettingsStrings::Medium::RT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_RD) {
    constexpr float rD = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RD + ": " + std::to_string(rD))).rD, rD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::RD + ": " + std::to_string(rD))).rD, rD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RD + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(rD))).rD, rD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::RD + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(rD))).rD, rD);
}

TEST(SettingsImporterMediumTests, MediumProperties_RDValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RD + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_RDSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RD + ": 1\n" + SettingsStrings::Medium::RD + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_RDT) {
    constexpr float rDT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RDT + ": " + std::to_string(rDT))).rDT, rDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::RDT + ": " + std::to_string(rDT))).rDT, rDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RDT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(rDT))).rDT, rDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::RDT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(rDT))).rDT, rDT);
}

TEST(SettingsImporterMediumTests, MediumProperties_RDTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RDT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_RDTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::RDT + ": 1\n" + SettingsStrings::Medium::RDT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_C0) {
    constexpr float c0 = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::C0 + ": " + std::to_string(c0))).c0, c0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::C0 + ": " + std::to_string(c0))).c0, c0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::C0 + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(c0))).c0, c0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::C0 + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(c0))).c0, c0);
}

TEST(SettingsImporterMediumTests, MediumProperties_C0ValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::C0 + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_C0SetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::C0 + ": 1\n" + SettingsStrings::Medium::C0 + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_CT) {
    constexpr float cT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CT + ": " + std::to_string(cT))).cT, cT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::CT + ": " + std::to_string(cT))).cT, cT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(cT))).cT, cT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::CT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(cT))).cT, cT);
}

TEST(SettingsImporterMediumTests, MediumProperties_CTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_CTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CT + ": 1\n" + SettingsStrings::Medium::CT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_CD) {
    constexpr float cD = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CD + ": " + std::to_string(cD))).cD, cD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::CD + ": " + std::to_string(cD))).cD, cD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CD + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(cD))).cD, cD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::CD + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(cD))).cD, cD);
}

TEST(SettingsImporterMediumTests, MediumProperties_CDValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CD + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_CDSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CD + ": 1\n" + SettingsStrings::Medium::CD + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_CDT) {
    constexpr float cDT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CDT + ": " + std::to_string(cDT))).cDT, cDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::CDT + ": " + std::to_string(cDT))).cDT, cDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CDT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(cDT))).cDT, cDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::CDT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(cDT))).cDT, cDT);
}

TEST(SettingsImporterMediumTests, MediumProperties_CDTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CDT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_CDTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::CDT + ": 1\n" + SettingsStrings::Medium::CDT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_K0) {
    constexpr float k0 = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::K0 + ": " + std::to_string(k0))).k0, k0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::K0 + ": " + std::to_string(k0))).k0, k0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::K0 + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(k0))).k0, k0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::K0 + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(k0))).k0, k0);
}

TEST(SettingsImporterMediumTests, MediumProperties_K0ValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::K0 + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_K0SetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::K0 + ": 1\n" + SettingsStrings::Medium::K0 + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_KT) {
    constexpr float kT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KT + ": " + std::to_string(kT))).kT, kT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::KT + ": " + std::to_string(kT))).kT, kT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(kT))).kT, kT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::KT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(kT))).kT, kT);
}

TEST(SettingsImporterMediumTests, MediumProperties_KTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_KTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KT + ": 1\n" + SettingsStrings::Medium::KT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_KD) {
    constexpr float kD = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KD + ": " + std::to_string(kD))).kD, kD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::KD + ": " + std::to_string(kD))).kD, kD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KD + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(kD))).kD, kD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::KD + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(kD))).kD, kD);
}

TEST(SettingsImporterMediumTests, MediumProperties_KDValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KD + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_KDSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KD + ": 1\n" + SettingsStrings::Medium::KD + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_KDT) {
    constexpr float kDT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KDT + ": " + std::to_string(kDT))).kDT, kDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::KDT + ": " + std::to_string(kDT))).kDT, kDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KDT + ":\n " + SettingsStrings::Yaml::Value + ": " + std::to_string(kDT))).kDT, kDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load(SettingsStrings::Yaml::Value + ":\n " + SettingsStrings::Medium::KDT + ":\n  " + SettingsStrings::Yaml::Value + ": " + std::to_string(kDT))).kDT, kDT);
}

TEST(SettingsImporterMediumTests, MediumProperties_KDTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KDT + ":\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_KDTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load(SettingsStrings::Medium::KDT + ": 1\n" + SettingsStrings::Medium::KDT + ": 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumPropertiesFromYaml_Full) {
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

    MediumProperties<float> properties(Physics_NS::MediumType::Arbitrary,
                                       n0, nT, nD, nDT,
                                       a0, aT, aD, aDT,
                                       u0, uT, uD, uDT,
                                       g0, gT, gD, gDT,
                                       r0, rT, rD, rDT,
                                       c0, cT, cD, cDT,
                                       k0, kT, kD, kDT);

    const auto yaml = mediumProperties(properties);
    const auto result = mediumProperties<float>(yaml);

    EXPECT_EQ      (properties.type       , result.type);
    EXPECT_FLOAT_EQ(properties.n0.value() , result.n0.value() );
    EXPECT_FLOAT_EQ(properties.nT.value() , result.nT.value() );
    EXPECT_FLOAT_EQ(properties.nD.value() , result.nD.value() );
    EXPECT_FLOAT_EQ(properties.nDT.value(), result.nDT.value());
    EXPECT_FLOAT_EQ(properties.a0.value() , result.a0.value() );
    EXPECT_FLOAT_EQ(properties.aT.value() , result.aT.value() );
    EXPECT_FLOAT_EQ(properties.aD.value() , result.aD.value() );
    EXPECT_FLOAT_EQ(properties.aDT.value(), result.aDT.value());
    EXPECT_FLOAT_EQ(properties.g0.value() , result.g0.value() );
    EXPECT_FLOAT_EQ(properties.gT.value() , result.gT.value() );
    EXPECT_FLOAT_EQ(properties.gD.value() , result.gD.value() );
    EXPECT_FLOAT_EQ(properties.gDT.value(), result.gDT.value());
    EXPECT_FLOAT_EQ(properties.r0.value() , result.r0.value() );
    EXPECT_FLOAT_EQ(properties.rT.value() , result.rT.value() );
    EXPECT_FLOAT_EQ(properties.rD.value() , result.rD.value() );
    EXPECT_FLOAT_EQ(properties.rDT.value(), result.rDT.value());
    EXPECT_FLOAT_EQ(properties.c0.value() , result.c0.value() );
    EXPECT_FLOAT_EQ(properties.cT.value() , result.cT.value() );
    EXPECT_FLOAT_EQ(properties.cD.value() , result.cD.value() );
    EXPECT_FLOAT_EQ(properties.cDT.value(), result.cDT.value());
    EXPECT_FLOAT_EQ(properties.k0.value() , result.k0.value() );
    EXPECT_FLOAT_EQ(properties.kT.value() , result.kT.value() );
    EXPECT_FLOAT_EQ(properties.kD.value() , result.kD.value() );
    EXPECT_FLOAT_EQ(properties.kDT.value(), result.kDT.value());
}
