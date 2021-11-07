#pragma once

#include "SettingsExporterHelpers.h"
#include "SettingsExporterLayer.h"
#include "SettingsExporterMedium.h"
#include "SettingsImporterLayer.h"

#include "../Utils/StringUtils.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace Physics_NS;
using namespace Settings_NS;
using namespace Utils_NS;
using namespace YAML;

TEST(SettingsExporterLayerTests, Glass) {
    constexpr int n0 = 2;
    constexpr int r0 = 6;
    constexpr int c0 = 7;
    constexpr int k0 = 8;
    constexpr int width = 100;

    MediumProperties<int> properties(MediumType::Glass,
                                     n0          , std::nullopt, std::nullopt, std::nullopt,
                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                     r0          , std::nullopt, std::nullopt, std::nullopt,
                                     c0          , std::nullopt, std::nullopt, std::nullopt,
                                     k0          , std::nullopt, std::nullopt, std::nullopt);

    const auto propertiesYaml = mediumProperties(properties);
    const auto mediumStr = to_string(propertiesYaml);
    const auto str = SettingsStrings::Layer::Begin + ": " + std::to_string(0    ) + "\n" +
                     SettingsStrings::Layer::End   + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Width + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Medium + ":\n" +
                     join(prepend(split(mediumStr, "\n"), "  "), "\n", false);

    const auto layer = layerFromYaml<int>(Load(str));

    const auto result = to_string(layerToyaml(layer));

    EXPECT_EQ(str, result);
}

TEST(SettingsExporterLayerTests, Constant) {
    constexpr int n0 = 2;
    constexpr int a0 = 3;
    constexpr int u0 = 4;
    constexpr int g0 = 5;
    constexpr int r0 = 6;
    constexpr int c0 = 7;
    constexpr int k0 = 8;
    constexpr int width = 100;

    MediumProperties<int> properties(MediumType::Constant,
                                     n0, std::nullopt, std::nullopt, std::nullopt,
                                     a0, std::nullopt, std::nullopt, std::nullopt,
                                     u0, std::nullopt, std::nullopt, std::nullopt,
                                     g0, std::nullopt, std::nullopt, std::nullopt,
                                     r0, std::nullopt, std::nullopt, std::nullopt,
                                     c0, std::nullopt, std::nullopt, std::nullopt,
                                     k0, std::nullopt, std::nullopt, std::nullopt);

    const auto propertiesYaml = mediumProperties(properties);
    const auto mediumStr = to_string(propertiesYaml);
    const auto str = SettingsStrings::Layer::Begin + ": " + std::to_string(0    ) + "\n" +
                     SettingsStrings::Layer::End   + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Width + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Medium + ":\n" +
                     join(prepend(split(mediumStr, "\n"), "  "), "\n", false);

    const auto layer = layerFromYaml<int>(Load(str));

    const auto result = to_string(layerToyaml(layer));

    EXPECT_EQ(str, result);
}

TEST(SettingsExporterLayerTests, Linear) {
    constexpr int n0 = 2;
    constexpr int nT = 3;
    constexpr int nD = 4;
    constexpr int a0 = 5;
    constexpr int aT = 6;
    constexpr int aD = 7;
    constexpr int u0 = 8;
    constexpr int uT = 9;
    constexpr int uD = 10;
    constexpr int g0 = 11;
    constexpr int gT = 12;
    constexpr int gD = 13;
    constexpr int r0 = 14;
    constexpr int rT = 15;
    constexpr int rD = 16;
    constexpr int c0 = 17;
    constexpr int cT = 18;
    constexpr int cD = 19;
    constexpr int k0 = 20;
    constexpr int kT = 21;
    constexpr int kD = 22;
    constexpr int width = 100;

    MediumProperties<int> properties(MediumType::Linear,
                                     n0, nT, nD, std::nullopt,
                                     a0, aT, aD, std::nullopt,
                                     u0, uT, uD, std::nullopt,
                                     g0, gT, gD, std::nullopt,
                                     r0, rT, rD, std::nullopt,
                                     c0, cT, cD, std::nullopt,
                                     k0, kT, kD, std::nullopt);

    const auto propertiesYaml = mediumProperties(properties);
    const auto mediumStr = to_string(propertiesYaml);
    const auto str = SettingsStrings::Layer::Begin + ": " + std::to_string(0    ) + "\n" +
                     SettingsStrings::Layer::End   + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Width + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Medium + ":\n" +
                     join(prepend(split(mediumStr, "\n"), "  "), "\n", false);

    const auto layer = layerFromYaml<int>(Load(str));

    const auto result = to_string(layerToyaml(layer));

    EXPECT_EQ(str, result);
}

TEST(SettingsExporterLayerTests, Arbitrary) {
    constexpr int n0  = 2;
    constexpr int nT  = 3;
    constexpr int nD  = 4;
    constexpr int nDT = 5;
    constexpr int a0  = 6;
    constexpr int aT  = 7;
    constexpr int aD  = 8;
    constexpr int aDT = 9;
    constexpr int u0  = 10;
    constexpr int uT  = 11;
    constexpr int uD  = 12;
    constexpr int uDT = 13;
    constexpr int g0  = 14;
    constexpr int gT  = 15;
    constexpr int gD  = 16;
    constexpr int gDT = 17;
    constexpr int r0  = 18;
    constexpr int rT  = 19;
    constexpr int rD  = 20;
    constexpr int rDT = 21;
    constexpr int c0  = 22;
    constexpr int cT  = 23;
    constexpr int cD  = 24;
    constexpr int cDT = 25;
    constexpr int k0  = 26;
    constexpr int kT  = 27;
    constexpr int kD  = 28;
    constexpr int kDT = 29;
    constexpr int width = 100;

    MediumProperties<int> properties(MediumType::Arbitrary,
                                     n0, nT, nD, nDT,
                                     a0, aT, aD, aDT,
                                     u0, uT, uD, uDT,
                                     g0, gT, gD, gDT,
                                     r0, rT, rD, rDT,
                                     c0, cT, cD, cDT,
                                     k0, kT, kD, kDT);

    const auto propertiesYaml = mediumProperties(properties);
    const auto mediumStr = to_string(propertiesYaml);
    const auto str = SettingsStrings::Layer::Begin + ": " + std::to_string(0    ) + "\n" +
                     SettingsStrings::Layer::End   + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Width + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Medium + ":\n" +
                     join(prepend(split(mediumStr, "\n"), "  "), "\n", false);

    const auto layer = layerFromYaml<int>(Load(str));

    const auto result = to_string(layerToyaml(layer));

    EXPECT_EQ(str, result);
}
