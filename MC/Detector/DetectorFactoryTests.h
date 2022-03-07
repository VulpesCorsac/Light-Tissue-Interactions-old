#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "DetectorFactory.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;
using namespace std;

TEST(DetectorFactoryTests, ThrowExceptionIfUnknownDetectorType) {
    auto properties = DetectorProperties<float>();
    properties.type = DetectorType::Unknown;
    EXPECT_THROW(auto medium = createDetector<float>(properties), invalid_argument);
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
