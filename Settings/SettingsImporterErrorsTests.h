#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "SettingsImporterErrors.h"

#include <gtest/gtest.h>

using namespace Settings_NS;

TEST(SettingsImporterErrorsTests, ThrowParsingErrorNoNode) {
    EXPECT_THROW(throwParsingErrorNoNode("", ""), std::runtime_error);
}
