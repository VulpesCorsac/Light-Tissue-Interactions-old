#pragma once

#include "MediumArbitrary.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;

class MediumArbitraryTests : public ::testing::Test {
protected:
    std::unique_ptr<MediumArbitrary<float>> medium = std::make_unique<MediumArbitrary<float>>();
};

TEST_F(MediumArbitraryTests, TypeIsArbitrary) {
    EXPECT_EQ(medium->type, MediumType::Arbitrary);
}
