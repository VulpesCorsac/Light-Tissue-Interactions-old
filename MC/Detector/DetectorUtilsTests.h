#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "DetectorUtils.h"

#include <gtest/gtest.h>

using namespace MonteCarlo_NS;

TEST(DetectorUtilsTests, DetectorTypeFromPtr_ThrownExceptionForNullptr) {
    EXPECT_THROW(detectorType<float>(nullptr), std::invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_ThrownExceptionForInterface) {
    std::unique_ptr<DetectorInterface<float>> detector = std::make_unique<DetectorInterface<float>>();
    EXPECT_THROW(detectorType<float>(detector.get()), std::invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_ThrownExceptionForUnknownType) {
    class DetectorDummy: public DetectorInterface<float> {};

    auto detector = std::make_unique<DetectorDummy>();
    EXPECT_THROW(detectorType<float>(detector.get()), std::invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_DetectorFullAbsorber) {
    auto detector = std::make_unique<FullAbsorber<float>>();
    EXPECT_EQ(detectorType<float>(detector.get()), DetectorType::FullAbsorber);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_DetectorIntegratingSphereSimple) {
    auto detector = std::make_unique<IntegratingSphereSimple<float>>();
    EXPECT_EQ(detectorType<float>(detector.get()), DetectorType::IntegratingSphereSimple);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_DetectorIntegratingSphereComplex) {
    auto detector = std::make_unique<IntegratingSphereComplex<float>>();
    EXPECT_EQ(detectorType<float>(detector.get()), DetectorType::IntegratingSphereComplex);
}

TEST(DetectorUtilsTests, DetectorTypeFromPtr_DetectorOpticalFiber) {
    auto detector = std::make_unique<OpticalFiber<float>>();
    EXPECT_EQ(detectorType<float>(detector.get()), DetectorType::OpticalFiber);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_ThrownExceptionForEmptyStr) {
    EXPECT_THROW(detectorType(""), std::invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_ThrownExceptionForUnknownType) {
    EXPECT_THROW(detectorType("InvalidDetector"), std::invalid_argument);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_DetectorFullAbsorber) {
    EXPECT_EQ(detectorType("fullabsorber" ), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("FullAbsorber" ), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("FULLABSORBER" ), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("full absorber"), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("Full Absorber"), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("FULL ABSORBER"), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("full_absorber"), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("Full_Absorber"), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("FULL_ABSORBER"), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("full-absorber"), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("Full-Absorber"), DetectorType::FullAbsorber);
    EXPECT_EQ(detectorType("FULL-ABSORBER"), DetectorType::FullAbsorber);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_DetectorIntegratingSphereSimple) {
    EXPECT_EQ(detectorType("integratingspheresimple"  ), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("IntegratingSphereSimple"  ), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("INTEGRATINGSPHERESIMPLE"  ), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("integrating sphere simple"), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("Integrating Sphere Simple"), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("INTEGRATING SPHERE SIMPLE"), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("integrating_sphere_simple"), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("Integrating_Sphere_Simple"), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("INTEGRATING_SPHERE_SIMPLE"), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("integrating-sphere-simple"), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("Integrating-Sphere-Simple"), DetectorType::IntegratingSphereSimple);
    EXPECT_EQ(detectorType("INTEGRATING-SPHERE-SIMPLE"), DetectorType::IntegratingSphereSimple);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_DetectorIntegratingSphereComplex) {
    EXPECT_EQ(detectorType("integratingspherecomplex"  ), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("IntegratingSphereComplex"  ), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("INTEGRATINGSPHERECOMPLEX"  ), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("integrating sphere complex"), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("Integrating Sphere Complex"), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("INTEGRATING SPHERE COMPLEX"), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("integrating_sphere_complex"), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("Integrating_Sphere_Complex"), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("INTEGRATING_SPHERE_COMPLEX"), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("integrating-sphere-complex"), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("Integrating-Sphere-Complex"), DetectorType::IntegratingSphereComplex);
    EXPECT_EQ(detectorType("INTEGRATING-SPHERE-COMPLEX"), DetectorType::IntegratingSphereComplex);
}

TEST(DetectorUtilsTests, DetectorTypeFromStr_DetectorOpticalFiber) {
    EXPECT_EQ(detectorType("opticalfiber" ), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("OpticalFiber" ), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("OPTICALFIBER" ), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("optical fiber"), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("Optical Fiber"), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("OPTICAL FIBER"), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("optical_fiber"), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("Optical_Fiber"), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("OPTICAL_FIBER"), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("optical-fiber"), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("Optical-Fiber"), DetectorType::OpticalFiber);
    EXPECT_EQ(detectorType("OPTICAL-FIBER"), DetectorType::OpticalFiber);
}

TEST(DetectorUtilsTests, DetectorTypeToStr_DetectorFullAbsorber) {
    EXPECT_EQ(to_string(DetectorType::FullAbsorber), "full_absorber");
}

TEST(DetectorUtilsTests, DetectorTypeToStr_DetectorIntegratingSphereSimple) {
    EXPECT_EQ(to_string(DetectorType::IntegratingSphereSimple), "integrating_sphere_simple");
}

TEST(DetectorUtilsTests, DetectorTypeToStr_DetectorIntegratingSphereComplex) {
    EXPECT_EQ(to_string(DetectorType::IntegratingSphereComplex), "integrating_sphere_complex");
}

TEST(DetectorUtilsTests, DetectorTypeToStr_DetectorOpticalFiber) {
    EXPECT_EQ(to_string(DetectorType::OpticalFiber), "optical_fiber");
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
    EXPECT_THROW(validate(properties), std::logic_error);
}

TEST(DetectorUtilsTests, ValidateSafe_ReturnsFalseForUnknownType) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::Unknown;
    EXPECT_FALSE(validateSafe(properties));
}

TEST(DetectorUtilsTests, Validate_ThrownExceptionForFullAbsorberWithoutCollimatedCosine) {
    DetectorProperties<int> properties;
    properties.type = DetectorType::FullAbsorber;
    EXPECT_THROW(validate(properties), std::logic_error);
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
    EXPECT_THROW(validate(properties), std::logic_error);
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
    EXPECT_THROW(validate(properties), std::logic_error);
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
    auto detector = std::make_unique<FullAbsorber<float>>(collimatedCosine);
    const auto& properties = exportDetectorProperties(detector.get());
    EXPECT_EQ(properties.type, DetectorType::FullAbsorber);
    EXPECT_FLOAT_EQ(properties.collimatedCosine.value(), collimatedCosine);
}
