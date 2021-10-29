#pragma once

#include "Layer.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

class LayerTests : public ::testing::Test {
protected:
    std::unique_ptr<MediumInterface<float>> medium = std::make_unique<MediumInterface<float>>();
};

TEST_F(LayerTests, ConstructorBeginEnd) {
    auto* raw = medium.get();

    auto layer = Layer(std::move(medium), 0, 1);

    EXPECT_FLOAT_EQ(layer.width, 1);

    EXPECT_EQ(layer.medium.get(), raw);
}

TEST_F(LayerTests, ConstructorWidth) {
    auto* raw = medium.get();

    auto layer = Layer(std::move(medium), 2);

    EXPECT_FLOAT_EQ(layer.width,  2);
    EXPECT_FLOAT_EQ(layer.begin, -1);
    EXPECT_FLOAT_EQ(layer.end  , -1);

    EXPECT_EQ(layer.medium.get(), raw);
}
