#pragma once

#include "MediumLinear.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

class MediumLinearTests : public ::testing::Test {
protected:
    MediumLinear<float>* const medium = new MediumLinear<float>();
};

TEST_F(MediumLinearTests, TypeIsLinear) {
    EXPECT_EQ(medium->type, MediumType::Linear);
}
