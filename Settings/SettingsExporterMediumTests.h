#pragma once

#include "SettingsExporter.h"
#include "SettingsExporterMedium.h"
#include "SettingsStrings.h"

#include <gtest/gtest.h>

using namespace Settings_NS;

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Unknown) {
    const auto result = mediumType(Physics_NS::MediumType::Unknown);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Unknown);
}

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Glass) {
    const auto result = mediumType(Physics_NS::MediumType::Glass);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Glass);
}

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Constant) {
    const auto result = mediumType(Physics_NS::MediumType::Constant);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Constant);
}

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Linear) {
    const auto result = mediumType(Physics_NS::MediumType::Linear);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Linear);
}

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Arbitrary) {
    const auto result = mediumType(Physics_NS::MediumType::Arbitrary);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Arbitrary);
}

TEST(SettingsExporterMediumTests, MediumTypeToYamlToType_Glass) {
    EXPECT_EQ(mediumType(mediumType(Physics_NS::MediumType::Glass)), Physics_NS::MediumType::Glass);
}

TEST(SettingsExporterMediumTests, MediumTypeToYamlToType_Constant) {
    EXPECT_EQ(mediumType(mediumType(Physics_NS::MediumType::Constant)), Physics_NS::MediumType::Constant);
}

TEST(SettingsExporterMediumTests, MediumTypeToYamlToType_Linear) {
    EXPECT_EQ(mediumType(mediumType(Physics_NS::MediumType::Linear)), Physics_NS::MediumType::Linear);
}

TEST(SettingsExporterMediumTests, MediumTypeToYamlToType_Arbitrary) {
    EXPECT_EQ(mediumType(mediumType(Physics_NS::MediumType::Arbitrary)), Physics_NS::MediumType::Arbitrary);
}
