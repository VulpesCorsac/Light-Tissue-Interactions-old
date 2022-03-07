#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "DetectorUtils.h"

#include "../../Utils/Utils.h"
#include "../../Utils/StringUtils.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;
using namespace Utils_NS;
using namespace std;

TEST(DetectorUtilsTests, DetectorTypeFromPtr_ThrownExceptionForNullptr) {
    EXPECT_THROW(detectorType<float>(nullptr), invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_ThrownExceptionForInterface) {
    unique_ptr<DetectorInterface<float>> detector = make_unique<DetectorInterface<float>>();
    EXPECT_THROW(detectorType<float>(detector.get()), invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_ThrownExceptionForUnknownType) {
    class DetectorDummy: public DetectorInterface<float> {};

    auto detector = make_unique<DetectorDummy>();
    EXPECT_THROW(detectorType<float>(detector.get()), invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_DetectorFullAbsorber) {
    auto detector = make_unique<FullAbsorber<float>>();
    EXPECT_EQ(detectorType<float>(detector.get()), DetectorType::FullAbsorber);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_DetectorIntegratingSphereSimple) {
    auto detector = make_unique<IntegratingSphereSimple<float>>();
    EXPECT_EQ(detectorType<float>(detector.get()), DetectorType::IntegratingSphereSimple);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_DetectorIntegratingSphereComplex) {
    auto detector = make_unique<IntegratingSphereComplex<float>>();
    EXPECT_EQ(detectorType<float>(detector.get()), DetectorType::IntegratingSphereComplex);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_DetectorOpticalFiber) {
    auto detector = make_unique<OpticalFiber<float>>();
    EXPECT_EQ(detectorType<float>(detector.get()), DetectorType::OpticalFiber);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_ThrownExceptionForEmptyStr) {
    EXPECT_THROW(detectorType(""), invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_ThrownExceptionForUnknownType) {
    EXPECT_THROW(detectorType("InvalidDetector"), invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_DetectorFullAbsorber) {
    auto type = DetectorTypeStrings::FullAbsorber;
    auto types = getAllVariants(to_lower(type));
    auto upper = getAllVariants(to_upper(type));
    types.insert(ALL_CONTAINER(upper));

    for (const auto& item: types)
        EXPECT_EQ(detectorType(item), DetectorType::FullAbsorber);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_DetectorIntegratingSphereSimple) {
    auto type = DetectorTypeStrings::IntegratingSphereSimple;
    auto types = getAllVariants(to_lower(type));
    auto upper = getAllVariants(to_upper(type));
    types.insert(ALL_CONTAINER(upper));

    for (const auto& item: types)
        EXPECT_EQ(detectorType(item), DetectorType::IntegratingSphereSimple);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_DetectorIntegratingSphereComplex) {
    auto type = DetectorTypeStrings::IntegratingSphereComplex;
    auto types = getAllVariants(to_lower(type));
    auto upper = getAllVariants(to_upper(type));
    types.insert(ALL_CONTAINER(upper));

    for (const auto& item: types)
        EXPECT_EQ(detectorType(item), DetectorType::IntegratingSphereComplex);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_DetectorOpticalFiber) {
    auto type = DetectorTypeStrings::OpticalFiber;
    auto types = getAllVariants(to_lower(type));
    auto upper = getAllVariants(to_upper(type));
    types.insert(ALL_CONTAINER(upper));

    for (const auto& item: types)
        EXPECT_EQ(detectorType(item), DetectorType::OpticalFiber);
}

TEST(DetectorUtilsTests, DetectorTypeToStr_DetectorFullAbsorber) {
    EXPECT_EQ(to_string(DetectorType::FullAbsorber), DetectorTypeStrings::FullAbsorber);
}

TEST(DetectorUtilsTests, DetectorTypeToStr_DetectorIntegratingSphereSimple) {
    EXPECT_EQ(to_string(DetectorType::IntegratingSphereSimple), DetectorTypeStrings::IntegratingSphereSimple);
}

TEST(DetectorUtilsTests, DetectorTypeToStr_DetectorIntegratingSphereComplex) {
    EXPECT_EQ(to_string(DetectorType::IntegratingSphereComplex), DetectorTypeStrings::IntegratingSphereComplex);
}

TEST(DetectorUtilsTests, DetectorTypeToStr_DetectorOpticalFiber) {
    EXPECT_EQ(to_string(DetectorType::OpticalFiber), DetectorTypeStrings::OpticalFiber);
}

TEST(DetectorUtilsTests, DetectorTypeToStrFromStr_DetectorFullAbsorber) {
    EXPECT_EQ(detectorType(to_string(DetectorType::FullAbsorber)), DetectorType::FullAbsorber);
}

TEST(DetectorUtilsTests, DetectorTypeToStrFromStr_DetectorIntegratingSphereSimple) {
    EXPECT_EQ(detectorType(to_string(DetectorType::IntegratingSphereSimple)), DetectorType::IntegratingSphereSimple);
}

TEST(DetectorUtilsTests, DetectorTypeToStrFromStr_DetectorIntegratingSphereComplex) {
    EXPECT_EQ(detectorType(to_string(DetectorType::IntegratingSphereComplex)), DetectorType::IntegratingSphereComplex);
}

TEST(DetectorUtilsTests, DetectorTypeToStrFromStr_DetectorOpticalFiber) {
    EXPECT_EQ(detectorType(to_string(DetectorType::OpticalFiber)), DetectorType::OpticalFiber);
}

TEST(DetectorUtilsTests, Validate_ThrownExceptionForUnknownType) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::Unknown;
    EXPECT_THROW(validate(properties), invalid_argument);
}

TEST(DetectorUtilsTests, ValidateSafe_ReturnsFalseForUnknownType) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::Unknown;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(DetectorUtilsTests, Validate_ThrownExceptionForFullAbsorberWithoutCollimatedCosine) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::FullAbsorber;
    EXPECT_THROW(validate(properties), invalid_argument);
}

TEST(DetectorUtilsTests, ValidateSafe_ReturnsFalseForFullAbsorberWithoutCollimatedCosine) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::FullAbsorber;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(DetectorUtilsTests, Validate_ThrownExceptionForFullAbsorberWithCollimatedCosineLessThanZero) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::FullAbsorber;
    properties.collimatedCosine = -1;
    EXPECT_THROW(validate(properties), invalid_argument);
}

TEST(DetectorUtilsTests, ValidateSafe_ReturnsFalseForFullAbsorberWithCollimatedCosineLessThanZero) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::FullAbsorber;
    properties.collimatedCosine = -1;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(DetectorUtilsTests, Validate_ThrownExceptionForFullAbsorberWithCollimatedCosineGreaterThanOne) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::FullAbsorber;
    properties.collimatedCosine = 2;
    EXPECT_THROW(validate(properties), invalid_argument);
}

TEST(DetectorUtilsTests, ValidateSafe_ReturnsFalseForFullAbsorberWithCollimatedCosineGreaterThanOne) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::FullAbsorber;
    properties.collimatedCosine = 2;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(DetectorUtilsTests, Validate_NoExceptionForProperFullAbsorber) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::FullAbsorber;
    properties.collimatedCosine = 0;
    EXPECT_NO_THROW(validate(properties));
}

TEST(DetectorUtilsTests, ValidateSafe_ReturnsTrueForProperFullAbsorber) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::FullAbsorber;
    properties.collimatedCosine = 0;
    EXPECT_TRUE(validateSafe(properties));
}

TEST(DetectorUtilsTests, ExportDetectorProperties_ForFullAbsorber) {
    static constexpr float collimatedCosine = 0.9;
    auto detector = make_unique<FullAbsorber<float>>(collimatedCosine);
    const auto& properties = exportDetectorProperties(detector.get());
    EXPECT_EQ(properties.type, DetectorType::FullAbsorber);
    EXPECT_FLOAT_EQ(properties.collimatedCosine.value(), collimatedCosine);
}
