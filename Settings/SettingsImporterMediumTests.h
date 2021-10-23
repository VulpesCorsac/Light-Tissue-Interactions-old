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

    MediumProperties<float> properties;
    properties.type = MediumType::Arbitrary;
    properties.n0   = n0;
    properties.nT   = nT;
    properties.nD   = nD;
    properties.nDT  = nDT;
    properties.a0   = a0;
    properties.aT   = aT;
    properties.aD   = aD;
    properties.aDT  = aDT;
    properties.u0   = u0;
    properties.uT   = uT;
    properties.uD   = uD;
    properties.uDT  = uDT;
    properties.g0   = g0;
    properties.gT   = gT;
    properties.gD   = gD;
    properties.gDT  = gDT;

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
}
