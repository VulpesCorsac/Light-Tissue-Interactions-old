#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "OpticalFiber.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;

class OpticalFiberTests : public ::testing::Test {
protected:
    std::unique_ptr<OpticalFiber<float>> detector = std::make_unique<OpticalFiber<float>>();
};

TEST_F(OpticalFiberTests, TypeIsOpticalFiber) {
    EXPECT_EQ(detector->type, DetectorType::OpticalFiber);
}

TEST_F(OpticalFiberTests, ThrowsExceptionForDetect) {
    EXPECT_THROW(detector->detect(Photon<float>()), std::runtime_error);
}

TEST_F(OpticalFiberTests, ThrowsExceptionForCalibrate) {
    EXPECT_THROW(detector->calibrate(1), std::runtime_error);
}
