#pragma once

#include "SettingsImporterLayer.h"

#include <gtest/gtest.h>

using namespace Settings_NS;
using namespace YAML;

TEST(SettingsImporterLayerTests, EmptyString) {
    EXPECT_THROW(layer<float>(Load("")), std::logic_error);
}

TEST(SettingsImporterLayerTests, WidthWithoutProperties) {
    EXPECT_THROW(layer<float>(Load(SettingsStrings::Layer::Width + ": 0")), std::logic_error);
    EXPECT_THROW(layer<float>(Load(SettingsStrings::Yaml::Value  + ":\n" +
                                   SettingsStrings::Layer::Width + ": 0")), std::logic_error);
}

TEST(SettingsImporterLayerTests, BeginAndEndWithoutProperties) {
    EXPECT_THROW(layer<float>(Load(SettingsStrings::Layer::Begin + ": 0" + "\n" +
                                   SettingsStrings::Layer::End   + ": 0")), std::logic_error);
    EXPECT_THROW(layer<float>(Load(SettingsStrings::Yaml::Value  + ":\n" +
                                   SettingsStrings::Layer::Begin + ": 0" + "\n" +
                                   SettingsStrings::Yaml::Value  + ":\n" +
                                   SettingsStrings::Layer::End   + ": 0")), std::logic_error);
}

TEST(SettingsImporterLayerTests, FullDeserialization) {

}
