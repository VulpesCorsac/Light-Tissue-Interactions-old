#pragma once

#include "SettingsExporter.h"
#include "SettingsExporterMedium.h"
#include "SettingsStrings.h"

#include <gtest/gtest.h>

using namespace Physics_NS;
using namespace Settings_NS;

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Unknown) {
    const auto result = mediumType(MediumType::Unknown);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + MediumTypeStrings::Unknown);
}

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Glass) {
    const auto result = mediumType(MediumType::Glass);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + MediumTypeStrings::Glass);
}

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Constant) {
    const auto result = mediumType(MediumType::Constant);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + MediumTypeStrings::Constant);
}

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Linear) {
    const auto result = mediumType(MediumType::Linear);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + MediumTypeStrings::Linear);
}

TEST(SettingsExporterMediumTests, MediumTypeToYaml_Arbitrary) {
    const auto result = mediumType(MediumType::Arbitrary);
    EXPECT_EQ(Settings_NS::to_string(result), SettingsStrings::Medium::Type + ": " + MediumTypeStrings::Arbitrary);
}

TEST(SettingsExporterMediumTests, MediumTypeToYamlToType_Glass) {
    EXPECT_EQ(mediumType(mediumType(MediumType::Glass)), MediumType::Glass);
}

TEST(SettingsExporterMediumTests, MediumTypeToYamlToType_Constant) {
    EXPECT_EQ(mediumType(mediumType(MediumType::Constant)), MediumType::Constant);
}

TEST(SettingsExporterMediumTests, MediumTypeToYamlToType_Linear) {
    EXPECT_EQ(mediumType(mediumType(MediumType::Linear)), MediumType::Linear);
}

TEST(SettingsExporterMediumTests, MediumTypeToYamlToType_Arbitrary) {
    EXPECT_EQ(mediumType(mediumType(MediumType::Arbitrary)), MediumType::Arbitrary);
}

TEST(SettingsExporterMediumTests, MediumPropertiesToYaml_Full) {
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

    const auto result = mediumProperties(properties);
    EXPECT_EQ(
        Settings_NS::to_string(result),
        SettingsStrings::Medium::Type + ": " + MediumTypeStrings::Arbitrary + "\n" +
        SettingsStrings::Medium::N0   + ": " + std::to_string(n0)           + "\n" +
        SettingsStrings::Medium::NT   + ": " + std::to_string(nT)           + "\n" +
        SettingsStrings::Medium::ND   + ": " + std::to_string(nD)           + "\n" +
        SettingsStrings::Medium::NDT  + ": " + std::to_string(nDT)          + "\n" +
        SettingsStrings::Medium::A0   + ": " + std::to_string(a0)           + "\n" +
        SettingsStrings::Medium::AT   + ": " + std::to_string(aT)           + "\n" +
        SettingsStrings::Medium::AD   + ": " + std::to_string(aD)           + "\n" +
        SettingsStrings::Medium::ADT  + ": " + std::to_string(aDT)          + "\n" +
        SettingsStrings::Medium::U0   + ": " + std::to_string(u0)           + "\n" +
        SettingsStrings::Medium::UT   + ": " + std::to_string(uT)           + "\n" +
        SettingsStrings::Medium::UD   + ": " + std::to_string(uD)           + "\n" +
        SettingsStrings::Medium::UDT  + ": " + std::to_string(uDT)          + "\n" +
        SettingsStrings::Medium::G0   + ": " + std::to_string(g0)           + "\n" +
        SettingsStrings::Medium::GT   + ": " + std::to_string(gT)           + "\n" +
        SettingsStrings::Medium::GD   + ": " + std::to_string(gD)           + "\n" +
        SettingsStrings::Medium::GDT  + ": " + std::to_string(gDT)
    );
}
