#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "SettingsImporterMediumType.h"

#include <gtest/gtest.h>

using namespace Settings_NS;

TEST(SettingsImporterTissueTypeTests, EmptyString) {
    EXPECT_THROW(mediumType(YAML::Load("")), std::invalid_argument);
}

TEST(SettingsImporterTissueTypeTests, InvalidType) {
    EXPECT_THROW(mediumType(YAML::Load("Kwa")), std::invalid_argument);
    EXPECT_THROW(mediumType(YAML::Load("value: Kwa")), std::invalid_argument);
}

TEST(SettingsImporterTissueTypeTests, Glass) {
    EXPECT_EQ(mediumType(YAML::Load("glass")), Physics_NS::MediumType::Glass);
    EXPECT_EQ(mediumType(YAML::Load("value: glass")), Physics_NS::MediumType::Glass);
}

TEST(SettingsImporterTissueTypeTests, Constant) {
    EXPECT_EQ(mediumType(YAML::Load("constant")), Physics_NS::MediumType::Constant);
    EXPECT_EQ(mediumType(YAML::Load("value: constant")), Physics_NS::MediumType::Constant);
}

TEST(SettingsImporterTissueTypeTests, Linear) {
    EXPECT_EQ(mediumType(YAML::Load("linear")), Physics_NS::MediumType::Linear);
    EXPECT_EQ(mediumType(YAML::Load("value: linear")), Physics_NS::MediumType::Linear);
}

TEST(SettingsImporterTissueTypeTests, Arbitrary) {
    EXPECT_EQ(mediumType(YAML::Load("arbitrary")), Physics_NS::MediumType::Arbitrary);
    EXPECT_EQ(mediumType(YAML::Load("value: arbitrary")), Physics_NS::MediumType::Arbitrary);
}
