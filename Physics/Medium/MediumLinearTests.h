#pragma once

#include "MediumLinear.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;

class MediumLinearTests : public ::testing::Test {
protected:
    std::unique_ptr<MediumLinear<float>> medium = std::make_unique<MediumLinear<float>>();
};

TEST_F(MediumLinearTests, TypeIsLinear) {
    EXPECT_EQ(medium->type, MediumType::Linear);
}
