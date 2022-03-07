#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "IntegratingSphereSimple.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;
using namespace std;

class IntegratingSphereSimpleTests : public ::testing::Test {
protected:
    unique_ptr<IntegratingSphereSimple<float>> detector = make_unique<IntegratingSphereSimple<float>>();
};

TEST_F(IntegratingSphereSimpleTests, TypeIsIntegratingSphereSimple) {
    EXPECT_EQ(detector->type, DetectorType::IntegratingSphereSimple);
}

TEST_F(IntegratingSphereSimpleTests, ThrowsExceptionForDetect) {
    EXPECT_THROW(detector->detect(Photon<float>()), runtime_error);
}

TEST_F(IntegratingSphereSimpleTests, ThrowsExceptionForCalibrate) {
    EXPECT_THROW(detector->calibrate(1), runtime_error);
}
