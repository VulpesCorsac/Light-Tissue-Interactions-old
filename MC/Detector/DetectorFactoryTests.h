#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "DetectorFactory.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;

TEST(DetectorFactoryTests, ThrowExceptionIfUnknownDetectorType) {
    auto properties = DetectorProperties<float>();
    properties.type = DetectorType::Unknown;
    EXPECT_THROW(auto medium = createDetector<float>(properties), std::invalid_argument);
}

TEST(DetectorFactoryTests, CreateFullAbsorberDetectorType) {
    auto properties = DetectorProperties<float>();
    properties.type = DetectorType::FullAbsorber;
    auto medium = createDetector<float>(properties);
    EXPECT_EQ(medium->type, properties.type);
}

TEST(DetectorFactoryTests, CreateIntegratingSphereSimpleDetectorType) {
    auto properties = DetectorProperties<float>();
    properties.type = DetectorType::IntegratingSphereSimple;
    auto medium = createDetector<float>(properties);
    EXPECT_EQ(medium->type, properties.type);
}

TEST(DetectorFactoryTests, CreateIntegratingSphereComplexDetectorType) {
    auto properties = DetectorProperties<float>();
    properties.type = DetectorType::IntegratingSphereComplex;
    auto medium = createDetector<float>(properties);
    EXPECT_EQ(medium->type, properties.type);
}

TEST(DetectorFactoryTests, CreateOpticalFiberDetectorType) {
    auto properties = DetectorProperties<float>();
    properties.type = DetectorType::OpticalFiber;
    auto medium = createDetector<float>(properties);
    EXPECT_EQ(medium->type, properties.type);
}