#pragma once

#include "SettingsImporterTissueTypeTests.h"

#include <gtest/gtest.h>

using namespace Settings_NS;

TEST(SettingsImporterTissueTypeTests, EmptyString) {
    /*
    const std::string configString = "";
    const auto config = YAML::Load(configString);
    EXPECT_THROW(mediumType(config), std::invalid_argument);
    //*/
}

TEST(SettingsImporterTissueTypeTests, Glass) {
    /*
    const std::string configString = "glass";
    const auto config = YAML::Load(configString);
    EXPECT_EQ(mediumType(config), Physics_NS::MediumType::Glass);
    //*/
}
