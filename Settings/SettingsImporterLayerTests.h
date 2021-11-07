#pragma once

#include "SettingsImporterLayer.h"

#include <gtest/gtest.h>

using namespace Physics_NS;
using namespace Settings_NS;
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

    MediumProperties<float> properties(MediumType::Glass,
                                       n0          , std::nullopt, std::nullopt, std::nullopt,
                                       std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                       std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                       std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                       r0          , std::nullopt, std::nullopt, std::nullopt,
                                       c0          , std::nullopt, std::nullopt, std::nullopt,
                                       k0          , std::nullopt, std::nullopt, std::nullopt);

    // const auto propertiesYaml = mediumProperties(properties);
    // std::string str = to_string(propertiesYaml);
}

TEST(SettingsImporterLayerTests, LayerFromYaml_Constant) {
}

TEST(SettingsImporterLayerTests, LayerFromYaml_Linear) {
}

TEST(SettingsImporterLayerTests, LayerFromYaml_Arbitrary) {
}
