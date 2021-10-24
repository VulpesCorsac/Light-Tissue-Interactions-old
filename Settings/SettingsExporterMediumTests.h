#pragma once

#include "SettingsExporter.h"
#include "SettingsExporterMedium.h"
#include "SettingsStrings.h"

#include <gtest/gtest.h>

using namespace Physics_NS;
using namespace Settings_NS;

#define TEST_MEDIUM_TYPE_TO_YAML(TYPE)                       \
TEST(SettingsExporterMediumTests, MediumTypeToYaml_##TYPE) { \
    const auto result = mediumType(MediumType::TYPE);        \
    EXPECT_EQ(to_string(result),                             \
              SettingsStrings::Medium::Type + ": " +         \
              MediumTypeStrings::TYPE);                      \
}

TEST_MEDIUM_TYPE_TO_YAML(Unknown)
TEST_MEDIUM_TYPE_TO_YAML(Glass)
TEST_MEDIUM_TYPE_TO_YAML(Constant)
TEST_MEDIUM_TYPE_TO_YAML(Linear)
TEST_MEDIUM_TYPE_TO_YAML(Arbitrary)

#define TEST_MEDIUM_TYPE_TO_YAML_TO_TYPE(TYPE)                     \
TEST(SettingsExporterMediumTests, MediumTypeToYamlToType_##TYPE) { \
    EXPECT_EQ(mediumType(mediumType(MediumType::TYPE)),            \
              MediumType::TYPE);                                   \
}

TEST_MEDIUM_TYPE_TO_YAML_TO_TYPE(Glass)
TEST_MEDIUM_TYPE_TO_YAML_TO_TYPE(Constant)
TEST_MEDIUM_TYPE_TO_YAML_TO_TYPE(Linear)
TEST_MEDIUM_TYPE_TO_YAML_TO_TYPE(Arbitrary)

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

    MediumProperties<float> properties(MediumType::Arbitrary,
                                       n0, nT, nD, nDT,
                                       a0, aT, aD, aDT,
                                       u0, uT, uD, uDT,
                                       g0, gT, gD, gDT,
                                       r0, rT, rD, rDT,
                                       c0, cT, cD, cDT,
                                       k0, kT, kD, kDT);

    const auto result = mediumProperties(properties);
    EXPECT_EQ(
        to_string(result),
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
        SettingsStrings::Medium::GDT  + ": " + std::to_string(gDT)          + "\n" +
        SettingsStrings::Medium::R0   + ": " + std::to_string(r0)           + "\n" +
        SettingsStrings::Medium::RT   + ": " + std::to_string(rT)           + "\n" +
        SettingsStrings::Medium::RD   + ": " + std::to_string(rD)           + "\n" +
        SettingsStrings::Medium::RDT  + ": " + std::to_string(rDT)          + "\n" +
        SettingsStrings::Medium::C0   + ": " + std::to_string(c0)           + "\n" +
        SettingsStrings::Medium::CT   + ": " + std::to_string(cT)           + "\n" +
        SettingsStrings::Medium::CD   + ": " + std::to_string(cD)           + "\n" +
        SettingsStrings::Medium::CDT  + ": " + std::to_string(cDT)          + "\n" +
        SettingsStrings::Medium::K0   + ": " + std::to_string(k0)           + "\n" +
        SettingsStrings::Medium::KT   + ": " + std::to_string(kT)           + "\n" +
        SettingsStrings::Medium::KD   + ": " + std::to_string(kD)           + "\n" +
        SettingsStrings::Medium::KDT  + ": " + std::to_string(kDT)
    );
}
