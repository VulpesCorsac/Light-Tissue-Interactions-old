#pragma once

#include "SettingsImporterErrors.h"

#include <gtest/gtest.h>

using namespace Settings_NS;

TEST(SettingsImporterErrorsTests, ThrowParsingErrorNoNode) {
    EXPECT_THROW(throwParsingErrorNoNode("", ""), std::runtime_error);
}
