#pragma once

#include "SettingsImporterMedium.h"
#include "SettingsExporterHelpers.h"
#include "SettingsExporterMedium.h"
#include "SettingsStrings.h"

#include <gtest/gtest.h>

using namespace Physics_NS;
using namespace Settings_NS;
using namespace std;

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

TEST(SettingsExporterMediumTests, MediumPropertiesToYaml_Glass) {
    constexpr float n0 = 2;
    constexpr float r0 = 6;
    constexpr float c0 = 7;
    constexpr float k0 = 8;

    MediumProperties<float> properties(MediumType::Glass,
                                       n0     , nullopt, nullopt, nullopt,
                                       nullopt, nullopt, nullopt, nullopt,
                                       nullopt, nullopt, nullopt, nullopt,
                                       nullopt, nullopt, nullopt, nullopt,
                                       r0     , nullopt, nullopt, nullopt,
                                       c0     , nullopt, nullopt, nullopt,
                                       k0     , nullopt, nullopt, nullopt);

    const auto result = mediumProperties(properties);
    EXPECT_EQ(
        to_string(result),
        SettingsStrings::Medium::Type + ": " + MediumTypeStrings::Glass + "\n" +
        SettingsStrings::Medium::N0   + ": " + to_string(n0)            + "\n" +
        SettingsStrings::Medium::R0   + ": " + to_string(r0)            + "\n" +
        SettingsStrings::Medium::C0   + ": " + to_string(c0)            + "\n" +
        SettingsStrings::Medium::K0   + ": " + to_string(k0)
    );
}

TEST(SettingsExporterMediumTests, MediumPropertiesToYaml_Constant) {
    constexpr float n0 = 2;
    constexpr float a0 = 3;
    constexpr float u0 = 4;
    constexpr float g0 = 5;
    constexpr float r0 = 6;
    constexpr float c0 = 7;
    constexpr float k0 = 8;

    MediumProperties<float> properties(MediumType::Constant,
                                       n0, nullopt, nullopt, nullopt,
                                       a0, nullopt, nullopt, nullopt,
                                       u0, nullopt, nullopt, nullopt,
                                       g0, nullopt, nullopt, nullopt,
                                       r0, nullopt, nullopt, nullopt,
                                       c0, nullopt, nullopt, nullopt,
                                       k0, nullopt, nullopt, nullopt);

    const auto result = mediumProperties(properties);
    EXPECT_EQ(
        to_string(result),
        SettingsStrings::Medium::Type + ": " + MediumTypeStrings::Constant + "\n" +
        SettingsStrings::Medium::N0   + ": " + to_string(n0)               + "\n" +
        SettingsStrings::Medium::A0   + ": " + to_string(a0)               + "\n" +
        SettingsStrings::Medium::U0   + ": " + to_string(u0)               + "\n" +
        SettingsStrings::Medium::G0   + ": " + to_string(g0)               + "\n" +
        SettingsStrings::Medium::R0   + ": " + to_string(r0)               + "\n" +
        SettingsStrings::Medium::C0   + ": " + to_string(c0)               + "\n" +
        SettingsStrings::Medium::K0   + ": " + to_string(k0)
    );
}

TEST(SettingsExporterMediumTests, MediumPropertiesToYaml_Linear) {
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

    MediumProperties<float> properties(MediumType::Linear,
                                       n0, nT, nD, nullopt,
                                       a0, aT, aD, nullopt,
                                       u0, uT, uD, nullopt,
                                       g0, gT, gD, nullopt,
                                       r0, rT, rD, nullopt,
                                       c0, cT, cD, nullopt,
                                       k0, kT, kD, nullopt);

    const auto result = mediumProperties(properties);
    EXPECT_EQ(
        to_string(result),
        SettingsStrings::Medium::Type + ": " + MediumTypeStrings::Linear + "\n" +
        SettingsStrings::Medium::N0   + ": " + to_string(n0)             + "\n" +
        SettingsStrings::Medium::NT   + ": " + to_string(nT)             + "\n" +
        SettingsStrings::Medium::ND   + ": " + to_string(nD)             + "\n" +
        SettingsStrings::Medium::A0   + ": " + to_string(a0)             + "\n" +
        SettingsStrings::Medium::AT   + ": " + to_string(aT)             + "\n" +
        SettingsStrings::Medium::AD   + ": " + to_string(aD)             + "\n" +
        SettingsStrings::Medium::U0   + ": " + to_string(u0)             + "\n" +
        SettingsStrings::Medium::UT   + ": " + to_string(uT)             + "\n" +
        SettingsStrings::Medium::UD   + ": " + to_string(uD)             + "\n" +
        SettingsStrings::Medium::G0   + ": " + to_string(g0)             + "\n" +
        SettingsStrings::Medium::GT   + ": " + to_string(gT)             + "\n" +
        SettingsStrings::Medium::GD   + ": " + to_string(gD)             + "\n" +
        SettingsStrings::Medium::R0   + ": " + to_string(r0)             + "\n" +
        SettingsStrings::Medium::RT   + ": " + to_string(rT)             + "\n" +
        SettingsStrings::Medium::RD   + ": " + to_string(rD)             + "\n" +
        SettingsStrings::Medium::C0   + ": " + to_string(c0)             + "\n" +
        SettingsStrings::Medium::CT   + ": " + to_string(cT)             + "\n" +
        SettingsStrings::Medium::CD   + ": " + to_string(cD)             + "\n" +
        SettingsStrings::Medium::K0   + ": " + to_string(k0)             + "\n" +
        SettingsStrings::Medium::KT   + ": " + to_string(kT)             + "\n" +
        SettingsStrings::Medium::KD   + ": " + to_string(kD)
    );
}

TEST(SettingsExporterMediumTests, MediumPropertiesToYaml_Arbitrary) {
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
        SettingsStrings::Medium::N0   + ": " + to_string(n0)                + "\n" +
        SettingsStrings::Medium::NT   + ": " + to_string(nT)                + "\n" +
        SettingsStrings::Medium::ND   + ": " + to_string(nD)                + "\n" +
        SettingsStrings::Medium::NDT  + ": " + to_string(nDT)               + "\n" +
        SettingsStrings::Medium::A0   + ": " + to_string(a0)                + "\n" +
        SettingsStrings::Medium::AT   + ": " + to_string(aT)                + "\n" +
        SettingsStrings::Medium::AD   + ": " + to_string(aD)                + "\n" +
        SettingsStrings::Medium::ADT  + ": " + to_string(aDT)               + "\n" +
        SettingsStrings::Medium::U0   + ": " + to_string(u0)                + "\n" +
        SettingsStrings::Medium::UT   + ": " + to_string(uT)                + "\n" +
        SettingsStrings::Medium::UD   + ": " + to_string(uD)                + "\n" +
        SettingsStrings::Medium::UDT  + ": " + to_string(uDT)               + "\n" +
        SettingsStrings::Medium::G0   + ": " + to_string(g0)                + "\n" +
        SettingsStrings::Medium::GT   + ": " + to_string(gT)                + "\n" +
        SettingsStrings::Medium::GD   + ": " + to_string(gD)                + "\n" +
        SettingsStrings::Medium::GDT  + ": " + to_string(gDT)               + "\n" +
        SettingsStrings::Medium::R0   + ": " + to_string(r0)                + "\n" +
        SettingsStrings::Medium::RT   + ": " + to_string(rT)                + "\n" +
        SettingsStrings::Medium::RD   + ": " + to_string(rD)                + "\n" +
        SettingsStrings::Medium::RDT  + ": " + to_string(rDT)               + "\n" +
        SettingsStrings::Medium::C0   + ": " + to_string(c0)                + "\n" +
        SettingsStrings::Medium::CT   + ": " + to_string(cT)                + "\n" +
        SettingsStrings::Medium::CD   + ": " + to_string(cD)                + "\n" +
        SettingsStrings::Medium::CDT  + ": " + to_string(cDT)               + "\n" +
        SettingsStrings::Medium::K0   + ": " + to_string(k0)                + "\n" +
        SettingsStrings::Medium::KT   + ": " + to_string(kT)                + "\n" +
        SettingsStrings::Medium::KD   + ": " + to_string(kD)                + "\n" +
        SettingsStrings::Medium::KDT  + ": " + to_string(kDT)
    );
}
