#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "SettingsImporterErrors.h"

#include <gtest/gtest.h>

using namespace Settings_NS;
using namespace std;

TEST(SettingsImporterErrorsTests, ThrowParsingErrorNoNode) {
    EXPECT_THROW(throwParsingErrorNoNode("", ""), runtime_error);
}
