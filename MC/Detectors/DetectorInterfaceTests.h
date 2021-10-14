#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "DetectorInterface.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;

class DetectorInterfaceTests : public ::testing::Test {
protected:
    std::unique_ptr<DetectorInterface<float>> medium = std::make_unique<DetectorInterface<float>>();
};

TEST_F(DetectorInterfaceTests, TypeIsUnknown) {
    EXPECT_EQ(medium->type, DetectorType::Unknown);
}

TEST_F(DetectorInterfaceTests, ThrowsExceptionForDetect) {
    EXPECT_THROW(medium->detect(Photon<float>()), std::runtime_error);
}
