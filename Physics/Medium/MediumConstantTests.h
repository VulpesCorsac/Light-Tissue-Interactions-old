#pragma once

#include "MediumConstant.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

class MediumConstantTests : public ::testing::Test {
protected:
    MediumConstant<float>* const medium = new MediumConstant<float>();
};

TEST_F(MediumConstantTests, TypeIsConstant) {
    EXPECT_EQ(medium->type, MediumType::Constant);
}
