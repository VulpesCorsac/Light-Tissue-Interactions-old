#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "IntegratingSphereSimple.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;

class IntegratingSphereSimpleTests : public ::testing::Test {
protected:
    std::unique_ptr<IntegratingSphereSimple<float>> detector = std::make_unique<IntegratingSphereSimple<float>>();
};

TEST_F(IntegratingSphereSimpleTests, TypeIsIntegratingSphereSimple) {
    EXPECT_EQ(detector->type, DetectorType::IntegratingSphereSimple);
}

TEST_F(IntegratingSphereSimpleTests, ThrowsExceptionForDetect) {
    EXPECT_THROW(detector->detect(Photon<float>()), std::runtime_error);
}

TEST_F(IntegratingSphereSimpleTests, ThrowsExceptionForCalibrate) {
    EXPECT_THROW(detector->calibrate(1), std::runtime_error);
}
