#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "IntegratingSphereComplex.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;

class IntegratingSphereComplexTests : public ::testing::Test {
protected:
    std::unique_ptr<IntegratingSphereComplex<float>> detector = std::make_unique<IntegratingSphereComplex<float>>();
};

TEST_F(IntegratingSphereComplexTests, TypeIsIntegratingSphereComplex) {
    EXPECT_EQ(detector->type, DetectorType::IntegratingSphereComplex);
}

TEST_F(IntegratingSphereComplexTests, ThrowsExceptionForDetect) {
    EXPECT_THROW(detector->detect(Photon<float>()), std::runtime_error);
}

TEST_F(IntegratingSphereComplexTests, ThrowsExceptionForCalibrate) {
    EXPECT_THROW(detector->calibrate(1), std::runtime_error);
}
