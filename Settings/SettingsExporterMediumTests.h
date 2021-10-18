#pragma once

#include "SettingsExporter.h"
#include "SettingsExporterMedium.h"
#include "SettingsStrings.h"

#include <gtest/gtest.h>

using namespace Settings_NS;

TEST(SettingsExporterMediumTests, MediumType_Unknown) {
    const auto result = mediumType(Physics_NS::MediumType::Unknown);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Unknown);
}

TEST(SettingsExporterMediumTests, MediumType_Glass) {
    const auto result = mediumType(Physics_NS::MediumType::Glass);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Glass);
}

TEST(SettingsExporterMediumTests, MediumType_Constant) {
    const auto result = mediumType(Physics_NS::MediumType::Constant);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Constant);
}

TEST(SettingsExporterMediumTests, MediumType_Linear) {
    const auto result = mediumType(Physics_NS::MediumType::Linear);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Linear);
}

TEST(SettingsExporterMediumTests, MediumType_Arbitrary) {
    const auto result = mediumType(Physics_NS::MediumType::Arbitrary);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + Physics_NS::MediumTypeStrings::Arbitrary);
}
