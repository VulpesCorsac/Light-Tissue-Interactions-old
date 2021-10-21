#pragma once

#ifdef ASSERT_INPUT_PARAMS
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

#include "DetectorInterface.h"
#include "DetectorProperties.h"
#include "DetectorType.h"
#include "DetectorTypeStrings.h"
#include "DistancesSweep.h"
#include "FullAbsorber.h"
#include "IntegratingSphereComplex.h"
#include "IntegratingSphereSimple.h"
#include "OpticalFiber.h"

#include "../../Utils/Utils.h"
#include "../../Utils/StringUtils.h"

#include <algorithm>
#include <stdexcept>
#include <string>

namespace MonteCarlo_NS {
    /// Detector type getter from DetectorInterface pointer
    /// \param[in] detector DetectorInterface pointer
    /// \return DetectorType, DetectorType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument If ASSERT_INPUT_PARAMS is defined and cannot convert to the known types
    template < typename T >
    inline DetectorType detectorType(DetectorInterface<T>* const detector) EXCEPT_INPUT_PARAMS;

    /// Detector type getter from DetectorInterface pointer
    /// \param[in] detector Detector name
    /// \return DetectorType, DetectorType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument If ASSERT_INPUT_PARAMS is defined and cannot convert to the known types
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
    if (dynamic_cast<MonteCarlo_NS::FullAbsorber<T>*>(detector))
        return MonteCarlo_NS::DetectorType::FullAbsorber;
    if (dynamic_cast<MonteCarlo_NS::IntegratingSphereSimple<T>*>(detector))
        return MonteCarlo_NS::DetectorType::IntegratingSphereSimple;
    if (dynamic_cast<MonteCarlo_NS::IntegratingSphereComplex<T>*>(detector))
        return MonteCarlo_NS::DetectorType::IntegratingSphereComplex;
    if (dynamic_cast<MonteCarlo_NS::OpticalFiber<T>*>(detector))
        return MonteCarlo_NS::DetectorType::OpticalFiber;

    #ifdef ASSERT_INPUT_PARAMS
        throw std::invalid_argument("Detector type cannot be evaluated");
    #endif // ASSERT_INPUT_PARAMS

    return MonteCarlo_NS::DetectorType::Unknown;
}

MonteCarlo_NS::DetectorType MonteCarlo_NS::detectorType(const std::string& detector) EXCEPT_INPUT_PARAMS {
    auto lower = Utils_NS::to_lower(detector);

    if (Utils_NS::contains(Utils_NS::getAllVariants(Utils_NS::to_lower(MonteCarlo_NS::DetectorTypeStrings::FullAbsorber)), lower))
        return MonteCarlo_NS::DetectorType::FullAbsorber;
    if (Utils_NS::contains(Utils_NS::getAllVariants(Utils_NS::to_lower(MonteCarlo_NS::DetectorTypeStrings::IntegratingSphereSimple)), lower))
        return MonteCarlo_NS::DetectorType::IntegratingSphereSimple;
    if (Utils_NS::contains(Utils_NS::getAllVariants(Utils_NS::to_lower(MonteCarlo_NS::DetectorTypeStrings::IntegratingSphereComplex)), lower))
        return MonteCarlo_NS::DetectorType::IntegratingSphereComplex;
    if (Utils_NS::contains(Utils_NS::getAllVariants(Utils_NS::to_lower(MonteCarlo_NS::DetectorTypeStrings::OpticalFiber)), lower))
        return MonteCarlo_NS::DetectorType::OpticalFiber;

    #ifdef ASSERT_INPUT_PARAMS
        throw std::invalid_argument("Medium type cannot be evaluated");
    #endif // ASSERT_INPUT_PARAMS

    return MonteCarlo_NS::DetectorType::Unknown;
}

std::string MonteCarlo_NS::to_string(const MonteCarlo_NS::DetectorType& detector) noexcept {
    switch (detector) {
    case MonteCarlo_NS::DetectorType::FullAbsorber:
        return MonteCarlo_NS::DetectorTypeStrings::FullAbsorber;
    case MonteCarlo_NS::DetectorType::IntegratingSphereSimple:
        return MonteCarlo_NS::DetectorTypeStrings::IntegratingSphereSimple;
    case MonteCarlo_NS::DetectorType::IntegratingSphereComplex:
        return MonteCarlo_NS::DetectorTypeStrings::IntegratingSphereComplex;
    case MonteCarlo_NS::DetectorType::OpticalFiber:
        return MonteCarlo_NS::DetectorTypeStrings::OpticalFiber;
    default:
        return MonteCarlo_NS::DetectorTypeStrings::Unknown;
    }
}

template < typename T >
void MonteCarlo_NS::validate(const MonteCarlo_NS::DetectorProperties<T>& properties) {
    if (properties.type == MonteCarlo_NS::DetectorType::Unknown)
        throw std::logic_error("Validation fails for DetectorType::Unknown");

    if (properties.type == MonteCarlo_NS::DetectorType::FullAbsorber) {
        if (!properties.collimatedCosine.has_value())
            throw std::logic_error("Validation fails for DetectorType::FullAbsorber because there is no collimatedCosine property");
        if (properties.collimatedCosine.value() < 0)
            throw std::logic_error("Validation fails for DetectorType::FullAbsorber because collimatedCosine property is less than 0");
        if (properties.collimatedCosine.value() > 1)
            throw std::logic_error("Validation fails for DetectorType::FullAbsorber because collimatedCosine property is greater than +1");
    }

    if (properties.type == MonteCarlo_NS::DetectorType::IntegratingSphereSimple) {
        return;
    }

    if (properties.type == MonteCarlo_NS::DetectorType::IntegratingSphereComplex) {
        return;
    }

    if (properties.type == MonteCarlo_NS::DetectorType::OpticalFiber) {
        return;
    }
}

template < typename T >
bool MonteCarlo_NS::validateSafe(const MonteCarlo_NS::DetectorProperties<T>& properties) noexcept {
    try {
        validate(properties);
        return true;
    } catch(std::logic_error&) {
        return false;
    }
}

template < typename T >
MonteCarlo_NS::DetectorProperties<T> MonteCarlo_NS::exportDetectorProperties(MonteCarlo_NS::DetectorInterface<T>* const detector) noexcept {
    MonteCarlo_NS::DetectorProperties<T> result;
    result.type = detectorType(detector);

    if (result.type == MonteCarlo_NS::DetectorType::FullAbsorber) {
        auto casted_detector = dynamic_cast<MonteCarlo_NS::FullAbsorber<T>*>(detector);
        result.collimatedCosine = casted_detector->collimatedCosine;
    }

    if (result.type == MonteCarlo_NS::DetectorType::IntegratingSphereSimple) {
        auto casted_detector = dynamic_cast<MonteCarlo_NS::IntegratingSphereSimple<T>*>(detector);
    }

    if (result.type == MonteCarlo_NS::DetectorType::IntegratingSphereComplex) {
        auto casted_detector = dynamic_cast<MonteCarlo_NS::IntegratingSphereComplex<T>*>(detector);
    }

    if (result.type == MonteCarlo_NS::DetectorType::OpticalFiber) {
        auto casted_detector = dynamic_cast<MonteCarlo_NS::OpticalFiber<T>*>(detector);
    }

    return result;
}
