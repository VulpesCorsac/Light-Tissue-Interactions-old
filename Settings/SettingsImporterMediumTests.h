#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "SettingsImporterMedium.h"

#include <gtest/gtest.h>

using namespace Settings_NS;

TEST(SettingsImporterMediumTests, MediumType_EmptyString) {
    EXPECT_THROW(mediumType(YAML::Load("")), std::invalid_argument);
}

TEST(SettingsImporterMediumTests, MediumType_InvalidType) {
    EXPECT_THROW(mediumType(YAML::Load("Kwa")), std::invalid_argument);
    EXPECT_THROW(mediumType(YAML::Load("value: Kwa")), std::invalid_argument);
}

TEST(SettingsImporterMediumTests, MediumType_Glass) {
    EXPECT_EQ(mediumType(YAML::Load("glass")), Physics_NS::MediumType::Glass);
    EXPECT_EQ(mediumType(YAML::Load("value: glass")), Physics_NS::MediumType::Glass);
}

TEST(SettingsImporterMediumTests, MediumType_Constant) {
    EXPECT_EQ(mediumType(YAML::Load("constant")), Physics_NS::MediumType::Constant);
    EXPECT_EQ(mediumType(YAML::Load("value: constant")), Physics_NS::MediumType::Constant);
}

TEST(SettingsImporterMediumTests, MediumType_Linear) {
    EXPECT_EQ(mediumType(YAML::Load("linear")), Physics_NS::MediumType::Linear);
    EXPECT_EQ(mediumType(YAML::Load("value: linear")), Physics_NS::MediumType::Linear);
}

TEST(SettingsImporterMediumTests, MediumType_Arbitrary) {
    EXPECT_EQ(mediumType(YAML::Load("arbitrary")), Physics_NS::MediumType::Arbitrary);
    EXPECT_EQ(mediumType(YAML::Load("value: arbitrary")), Physics_NS::MediumType::Arbitrary);
}

TEST(SettingsImporterMediumTests, MediumProperties_MediumTypeGlass) {
    EXPECT_EQ(mediumProperties<float>(YAML::Load("medium_type: glass")).type, Physics_NS::MediumType::Glass);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n medium_type: glass")).type, Physics_NS::MediumType::Glass);
}

TEST(SettingsImporterMediumTests, MediumProperties_MediumTypeConstant) {
    EXPECT_EQ(mediumProperties<float>(YAML::Load("medium_type: constant")).type, Physics_NS::MediumType::Constant);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n medium_type: constant")).type, Physics_NS::MediumType::Constant);
}

TEST(SettingsImporterMediumTests, MediumProperties_MediumTypeLinear) {
    EXPECT_EQ(mediumProperties<float>(YAML::Load("medium_type: linear")).type, Physics_NS::MediumType::Linear);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n medium_type: linear")).type, Physics_NS::MediumType::Linear);
}

TEST(SettingsImporterMediumTests, MediumProperties_MediumTypeArbitrary) {
    EXPECT_EQ(mediumProperties<float>(YAML::Load("medium_type: arbitrary")).type, Physics_NS::MediumType::Arbitrary);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n medium_type: arbitrary")).type, Physics_NS::MediumType::Arbitrary);
}

TEST(SettingsImporterMediumTests, MediumProperties_N0) {
    constexpr float n0 = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load("n0: " + std::to_string(n0))).n0, n0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n n0: " + std::to_string(n0))).n0, n0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("n0:\n value: " + std::to_string(n0))).n0, n0);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n n0:\n  value: " + std::to_string(n0))).n0, n0);
}

TEST(SettingsImporterMediumTests, MediumProperties_N0ValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load("n0:\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_N0SetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load("n0: 1\nn0: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NT) {
    constexpr float nT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load("nT: " + std::to_string(nT))).nT, nT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n nT: " + std::to_string(nT))).nT, nT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("nT:\n value: " + std::to_string(nT))).nT, nT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n nT:\n  value: " + std::to_string(nT))).nT, nT);
}

TEST(SettingsImporterMediumTests, MediumProperties_NTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load("nT:\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load("nT: 1\nnT: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_ND) {
    constexpr float nD = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load("nD: " + std::to_string(nD))).nD, nD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n nD: " + std::to_string(nD))).nD, nD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("nD:\n value: " + std::to_string(nD))).nD, nD);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n nD:\n  value: " + std::to_string(nD))).nD, nD);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load("nD:\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load("nD: 1\nnD: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDT) {
    constexpr float nDT = 1;
    EXPECT_EQ(mediumProperties<float>(YAML::Load("nDT: " + std::to_string(nDT))).nDT, nDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n nDT: " + std::to_string(nDT))).nDT, nDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("nDT:\n value: " + std::to_string(nDT))).nDT, nDT);
    EXPECT_EQ(mediumProperties<float>(YAML::Load("value:\n nDT:\n  value: " + std::to_string(nDT))).nDT, nDT);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDTValueNotScalar) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load("nDT:\n kwa: 1")), std::logic_error);
}

TEST(SettingsImporterMediumTests, MediumProperties_NDTSetTwice) {
    EXPECT_THROW(mediumProperties<float>(YAML::Load("nDT: 1\nnDT: 1")), std::logic_error);
}
