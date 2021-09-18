#pragma once

#include "MediumConstant.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;

class MediumConstantTests : public ::testing::Test {
protected:
    std::unique_ptr<MediumConstant<float>> medium = std::make_unique<MediumConstant<float>>();
};

TEST_F(MediumConstantTests, TypeIsConstant) {
    EXPECT_EQ(medium->type, MediumType::Constant);
}
