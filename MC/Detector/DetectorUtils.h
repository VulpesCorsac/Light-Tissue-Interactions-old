#pragma once

#include "DetectorInterface.h"
#include "DetectorProperties.h"
#include "DetectorType.h"
#include "DetectorTypeStrings.h"
#include "DistancesSweep.h"
#include "FullAbsorber.h"
#include "IntegratingSphereComplex.h"
#include "IntegratingSphereSimple.h"
#include "OpticalFiber.h"

#include "../../Utils/Contracts.h"
#include "../../Utils/Utils.h"
#include "../../Utils/StringUtils.h"

#include <algorithm>
#include <string>

namespace MonteCarlo_NS {
    /// Detector type getter from DetectorInterface pointer
    /// \param[in] detector DetectorInterface pointer
    /// \return DetectorType, DetectorType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument if ENABLE_CHECK_CONTRACTS is defined and cannot convert to the known types
    template < typename T >
    inline DetectorType detectorType(DetectorInterface<T>* const detector) EXCEPT_INPUT_PARAMS;

    /// Detector type getter from DetectorInterface pointer
    /// \param[in] detector Detector name
    /// \return DetectorType, DetectorType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument if ENABLE_CHECK_CONTRACTS is defined and cannot convert to the known types
    inline DetectorType detectorType(const std::string& detector) EXCEPT_INPUT_PARAMS;

    /// Get detector type string representation
    /// \param[in] detector DetectorType
    /// \return string representing detector type
    inline std::string to_string(const DetectorType& detector) noexcept;

    /// Validator for DetectorProperties
    /// \param[in] properties DetectorProperties
    /// \return in case validation succeeded no exception or value is returned
    /// \throw std::logic_error if not all needed properties are available
    template < typename T >
    void validate(const DetectorProperties<T>& properties);
    /// Validator for DetectorProperties, calls validate
    /// \param[in] properties DetectorProperties
    /// \return true if validation succeeded no exception or false otherwise
    template < typename T >
    bool validateSafe(const DetectorProperties<T>& properties) noexcept;

    /// Export detector properties to DetectorProperties
    /// \param[in] detector DetectorInterface pointer
    /// \return DetectorProperties for corresponding detector
    template < typename T >
    DetectorProperties<T> exportDetectorProperties(DetectorInterface<T>* const detector) noexcept;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
MonteCarlo_NS::DetectorType MonteCarlo_NS::detectorType(MonteCarlo_NS::DetectorInterface<T>* const detector) EXCEPT_INPUT_PARAMS {
    if (dynamic_cast<FullAbsorber<T>*>(detector))
        return DetectorType::FullAbsorber;
    if (dynamic_cast<IntegratingSphereSimple<T>*>(detector))
        return DetectorType::IntegratingSphereSimple;
    if (dynamic_cast<IntegratingSphereComplex<T>*>(detector))
        return DetectorType::IntegratingSphereComplex;
    if (dynamic_cast<OpticalFiber<T>*>(detector))
        return DetectorType::OpticalFiber;

    FAIL_ARGUMENT_CONTRACT("Detector type cannot be evaluated");

    return DetectorType::Unknown;
}

MonteCarlo_NS::DetectorType MonteCarlo_NS::detectorType(const std::string& detector) EXCEPT_INPUT_PARAMS {
    using namespace Utils_NS;

    auto lower = to_lower(detector);

    if (contains(getAllVariants(to_lower(DetectorTypeStrings::FullAbsorber)), lower))
        return DetectorType::FullAbsorber;
    if (contains(getAllVariants(to_lower(DetectorTypeStrings::IntegratingSphereSimple)), lower))
        return DetectorType::IntegratingSphereSimple;
    if (contains(getAllVariants(to_lower(DetectorTypeStrings::IntegratingSphereComplex)), lower))
        return DetectorType::IntegratingSphereComplex;
    if (contains(getAllVariants(to_lower(DetectorTypeStrings::OpticalFiber)), lower))
        return DetectorType::OpticalFiber;

    FAIL_ARGUMENT_CONTRACT("Medium type cannot be evaluated");

    return DetectorType::Unknown;
}

std::string MonteCarlo_NS::to_string(const MonteCarlo_NS::DetectorType& detector) noexcept {
    switch (detector) {
        case DetectorType::FullAbsorber:
            return DetectorTypeStrings::FullAbsorber;
        case DetectorType::IntegratingSphereSimple:
            return DetectorTypeStrings::IntegratingSphereSimple;
        case DetectorType::IntegratingSphereComplex:
            return DetectorTypeStrings::IntegratingSphereComplex;
        case DetectorType::OpticalFiber:
            return DetectorTypeStrings::OpticalFiber;
        default:
            return DetectorTypeStrings::Unknown;
    }
}

template < typename T >
void MonteCarlo_NS::validate(const MonteCarlo_NS::DetectorProperties<T>& properties) {
    CHECK_ARGUMENT_CONTRACT(properties.type != DetectorType::Unknown);

    if (properties.type == DetectorType::FullAbsorber) {
        CHECK_ARGUMENT_CONTRACT(properties.collimatedCosine.has_value());
        CHECK_ARGUMENT_CONTRACT(properties.collimatedCosine.value() >= 0);
        CHECK_ARGUMENT_CONTRACT(properties.collimatedCosine.value() <= 1);
    }

    if (properties.type == DetectorType::IntegratingSphereSimple) {
        return;
    }

    if (properties.type == DetectorType::IntegratingSphereComplex) {
        return;
    }

    if (properties.type == DetectorType::OpticalFiber) {
        return;
    }
}

template < typename T >
bool MonteCarlo_NS::validateSafe(const MonteCarlo_NS::DetectorProperties<T>& properties) noexcept {
    using namespace std;

    try {
        validate(properties);
        return true;
    } catch(invalid_argument&) {
        return false;
    }
}

template < typename T >
MonteCarlo_NS::DetectorProperties<T> MonteCarlo_NS::exportDetectorProperties(MonteCarlo_NS::DetectorInterface<T>* const detector) noexcept {
    DetectorProperties<T> result;
    result.type = detectorType(detector);

    if (result.type == DetectorType::FullAbsorber)
        if (auto casted_detector = dynamic_cast<FullAbsorber<T>*>(detector)) {
            result.collimatedCosine = casted_detector->collimatedCosine;
        }

    if (result.type == DetectorType::IntegratingSphereSimple)
        if (auto casted_detector = dynamic_cast<IntegratingSphereSimple<T>*>(detector)) {
        }

    if (result.type == DetectorType::IntegratingSphereComplex)
        if (auto casted_detector = dynamic_cast<IntegratingSphereComplex<T>*>(detector)) {
        }

    if (result.type == DetectorType::OpticalFiber)
        if (auto casted_detector = dynamic_cast<OpticalFiber<T>*>(detector)) {
        }

    return result;
}
