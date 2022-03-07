#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "OpticalFiber.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;
using namespace std;

class OpticalFiberTests : public ::testing::Test {
protected:
    unique_ptr<OpticalFiber<float>> detector = make_unique<OpticalFiber<float>>();
};

TEST_F(OpticalFiberTests, TypeIsOpticalFiber) {
    EXPECT_EQ(detector->type, DetectorType::OpticalFiber);
}

TEST_F(OpticalFiberTests, ThrowsExceptionForDetect) {
    EXPECT_THROW(detector->detect(Photon<float>()), runtime_error);
}

TEST_F(OpticalFiberTests, ThrowsExceptionForCalibrate) {
    EXPECT_THROW(detector->calibrate(1), runtime_error);
}
