#pragma once

#include "SettingsExporterHelpers.h"
#include "SettingsExporterMedium.h"
#include "SettingsImporterLayer.h"

#include "../Utils/StringUtils.h"

#include <gtest/gtest.h>

using namespace Physics_NS;
using namespace Settings_NS;
using namespace Utils_NS;
using namespace YAML;

TEST(SettingsImporterLayerTests, EmptyString) {
    EXPECT_THROW(layerFromYaml<float>(Load("")), std::logic_error);
}

TEST(SettingsImporterLayerTests, WidthWithoutProperties) {
    EXPECT_THROW(layerFromYaml<float>(Load(SettingsStrings::Layer::Width + ": 0")), std::logic_error);
    EXPECT_THROW(layerFromYaml<float>(Load(SettingsStrings::Yaml::Value  + ":\n" +
                                           SettingsStrings::Layer::Width + ": 0")), std::logic_error);
}

TEST(SettingsImporterLayerTests, BeginAndEndWithoutProperties) {
    EXPECT_THROW(layerFromYaml<float>(Load(SettingsStrings::Layer::Begin + ": 0" + "\n" +
                                           SettingsStrings::Layer::End   + ": 0")), std::logic_error);
    EXPECT_THROW(layerFromYaml<float>(Load(SettingsStrings::Yaml::Value  + ":\n" +
                                           SettingsStrings::Layer::Begin + ": 0" + "\n" +
                                           SettingsStrings::Yaml::Value  + ":\n" +
                                           SettingsStrings::Layer::End   + ": 0")), std::logic_error);
}

TEST(SettingsImporterLayerTests, LayerFromYaml_Glass) {
    constexpr float n0 = 2;
    constexpr float r0 = 6;
    constexpr float c0 = 7;
    constexpr float k0 = 8;
    constexpr float width = 100;

    MediumProperties<float> properties(MediumType::Glass,
                                       n0          , std::nullopt, std::nullopt, std::nullopt,
                                       std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                       std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                       std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                       r0          , std::nullopt, std::nullopt, std::nullopt,
                                       c0          , std::nullopt, std::nullopt, std::nullopt,
                                       k0          , std::nullopt, std::nullopt, std::nullopt);

    const auto propertiesYaml = mediumProperties(properties);
    const auto mediumStr = to_string(propertiesYaml);
    const auto str = SettingsStrings::Layer::Width + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Medium + ":\n" +
                     join(prepend(split(mediumStr, "\n"), "  "), "\n", true);

    const auto layer = layerFromYaml<float>(Load(str));

    EXPECT_EQ(layer.width, width);

    const auto result = exportMediumProperties(layer.medium.get());

    EXPECT_EQ      (properties.type      , result.type      );
    EXPECT_FLOAT_EQ(properties.n0.value(), result.n0.value());
    EXPECT_FLOAT_EQ(properties.r0.value(), result.r0.value());
    EXPECT_FLOAT_EQ(properties.c0.value(), result.c0.value());
    EXPECT_FLOAT_EQ(properties.k0.value(), result.k0.value());
}

TEST(SettingsImporterLayerTests, LayerFromYaml_Constant) {
    constexpr float n0 = 2;
    constexpr float a0 = 3;
    constexpr float u0 = 4;
    constexpr float g0 = 5;
    constexpr float r0 = 6;
    constexpr float c0 = 7;
    constexpr float k0 = 8;
    constexpr float width = 100;

    MediumProperties<float> properties(MediumType::Constant,
                                       n0, std::nullopt, std::nullopt, std::nullopt,
                                       a0, std::nullopt, std::nullopt, std::nullopt,
                                       u0, std::nullopt, std::nullopt, std::nullopt,
                                       g0, std::nullopt, std::nullopt, std::nullopt,
                                       r0, std::nullopt, std::nullopt, std::nullopt,
                                       c0, std::nullopt, std::nullopt, std::nullopt,
                                       k0, std::nullopt, std::nullopt, std::nullopt);

    const auto propertiesYaml = mediumProperties(properties);
    const auto mediumStr = to_string(propertiesYaml);
    const auto str = SettingsStrings::Layer::Width + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Medium + ":\n" +
                     join(prepend(split(mediumStr, "\n"), "  "), "\n", true);

    const auto layer = layerFromYaml<float>(Load(str));

    EXPECT_EQ(layer.width, width);

    const auto result = exportMediumProperties(layer.medium.get());

    EXPECT_EQ      (properties.type      , result.type      );
    EXPECT_FLOAT_EQ(properties.n0.value(), result.n0.value());
    EXPECT_FLOAT_EQ(properties.a0.value(), result.a0.value());
    EXPECT_FLOAT_EQ(properties.u0.value(), result.u0.value());
    EXPECT_FLOAT_EQ(properties.g0.value(), result.g0.value());
    EXPECT_FLOAT_EQ(properties.r0.value(), result.r0.value());
    EXPECT_FLOAT_EQ(properties.c0.value(), result.c0.value());
    EXPECT_FLOAT_EQ(properties.k0.value(), result.k0.value());
}

TEST(SettingsImporterLayerTests, LayerFromYaml_Linear) {
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
    constexpr float width = 100;

    MediumProperties<float> properties(MediumType::Linear,
                                       n0, nT, nD, std::nullopt,
                                       a0, aT, aD, std::nullopt,
                                       u0, uT, uD, std::nullopt,
                                       g0, gT, gD, std::nullopt,
                                       r0, rT, rD, std::nullopt,
                                       c0, cT, cD, std::nullopt,
                                       k0, kT, kD, std::nullopt);

    const auto propertiesYaml = mediumProperties(properties);
    const auto mediumStr = to_string(propertiesYaml);
    const auto str = SettingsStrings::Layer::Width + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Medium + ":\n" +
                     join(prepend(split(mediumStr, "\n"), "  "), "\n", true);

    const auto layer = layerFromYaml<float>(Load(str));

    EXPECT_EQ(layer.width, width);

    const auto result = exportMediumProperties(layer.medium.get());

    EXPECT_EQ      (properties.type      , result.type      );
    EXPECT_FLOAT_EQ(properties.n0.value(), result.n0.value());
    EXPECT_FLOAT_EQ(properties.nT.value(), result.nT.value());
    EXPECT_FLOAT_EQ(properties.nD.value(), result.nD.value());
    EXPECT_FLOAT_EQ(properties.a0.value(), result.a0.value());
    EXPECT_FLOAT_EQ(properties.aT.value(), result.aT.value());
    EXPECT_FLOAT_EQ(properties.aD.value(), result.aD.value());
    EXPECT_FLOAT_EQ(properties.u0.value(), result.u0.value());
    EXPECT_FLOAT_EQ(properties.uT.value(), result.uT.value());
    EXPECT_FLOAT_EQ(properties.uD.value(), result.uD.value());
    EXPECT_FLOAT_EQ(properties.g0.value(), result.g0.value());
    EXPECT_FLOAT_EQ(properties.gT.value(), result.gT.value());
    EXPECT_FLOAT_EQ(properties.gD.value(), result.gD.value());
    EXPECT_FLOAT_EQ(properties.r0.value(), result.r0.value());
    EXPECT_FLOAT_EQ(properties.rT.value(), result.rT.value());
    EXPECT_FLOAT_EQ(properties.rD.value(), result.rD.value());
    EXPECT_FLOAT_EQ(properties.c0.value(), result.c0.value());
    EXPECT_FLOAT_EQ(properties.cT.value(), result.cT.value());
    EXPECT_FLOAT_EQ(properties.cD.value(), result.cD.value());
    EXPECT_FLOAT_EQ(properties.k0.value(), result.k0.value());
    EXPECT_FLOAT_EQ(properties.kT.value(), result.kT.value());
    EXPECT_FLOAT_EQ(properties.kD.value(), result.kD.value());
}

TEST(SettingsImporterLayerTests, LayerFromYaml_Arbitrary) {
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
    constexpr float width = 100;

    MediumProperties<float> properties(MediumType::Arbitrary,
                                       n0, nT, nD, nDT,
                                       a0, aT, aD, aDT,
                                       u0, uT, uD, uDT,
                                       g0, gT, gD, gDT,
                                       r0, rT, rD, rDT,
                                       c0, cT, cD, cDT,
                                       k0, kT, kD, kDT);

    const auto propertiesYaml = mediumProperties(properties);
    const auto mediumStr = to_string(propertiesYaml);
    const auto str = SettingsStrings::Layer::Width + ": " + std::to_string(width) + "\n" +
                     SettingsStrings::Layer::Medium + ":\n" +
                     join(prepend(split(mediumStr, "\n"), "  "), "\n", true);

    const auto layer = layerFromYaml<float>(Load(str));

    EXPECT_EQ(layer.width, width);

    const auto result = exportMediumProperties(layer.medium.get());

    EXPECT_EQ      (properties.type       , result.type       );
    EXPECT_FLOAT_EQ(properties.n0.value() , result.n0.value() );
    EXPECT_FLOAT_EQ(properties.nT.value() , result.nT.value() );
    EXPECT_FLOAT_EQ(properties.nD.value() , result.nD.value() );
    EXPECT_FLOAT_EQ(properties.nDT.value(), result.nDT.value());
    EXPECT_FLOAT_EQ(properties.a0.value() , result.a0.value() );
    EXPECT_FLOAT_EQ(properties.aT.value() , result.aT.value() );
    EXPECT_FLOAT_EQ(properties.aD.value() , result.aD.value() );
    EXPECT_FLOAT_EQ(properties.aDT.value(), result.aDT.value());
    EXPECT_FLOAT_EQ(properties.u0.value() , result.u0.value() );
    EXPECT_FLOAT_EQ(properties.uT.value() , result.uT.value() );
    EXPECT_FLOAT_EQ(properties.uD.value() , result.uD.value() );
    EXPECT_FLOAT_EQ(properties.uDT.value(), result.uDT.value());
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
