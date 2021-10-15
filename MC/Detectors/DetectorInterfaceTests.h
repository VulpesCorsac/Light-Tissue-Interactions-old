#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "DetectorInterface.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;

class DetectorInterfaceTests : public ::testing::Test {
protected:
    std::unique_ptr<DetectorInterface<float>> detector = std::make_unique<DetectorInterface<float>>();
};

TEST_F(DetectorInterfaceTests, TypeIsUnknown) {
    EXPECT_EQ(detector->type, DetectorType::Unknown);
}

TEST_F(DetectorInterfaceTests, Constructor) {
    std::unique_ptr<DetectorInterface<float>> generatedDetector = std::make_unique<DetectorInterface<float>>(DetectorType::FullAbsorber);
    EXPECT_EQ(generatedDetector->type, DetectorType::FullAbsorber);
}

TEST_F(DetectorInterfaceTests, ThrowsExceptionForDetect) {
    EXPECT_THROW(detector->detect(Photon<float>()), std::runtime_error);
}

TEST_F(DetectorInterfaceTests, ThrowsExceptionForCalibrate) {
    EXPECT_THROW(detector->calibrate(1), std::runtime_error);
}
