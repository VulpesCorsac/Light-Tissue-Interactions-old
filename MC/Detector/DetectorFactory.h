#pragma once

#include "DetectorInterface.h"
#include "DetectorProperties.h"
#include "DetectorType.h"
#include "FullAbsorber.h"
#include "IntegratingSphereComplex.h"
#include "IntegratingSphereSimple.h"
#include "OpticalFiber.h"

#include "../../Utils/Contracts.h"

#include <memory>

namespace MonteCarlo_NS {
    /// \brief factory function to create detector of needed type based on properties
    /// \param[in] properties DetectorProperties
    /// \return detector of needed type based on properties
    /// \throw std::invalid_argument if ENABLE_CHECK_CONTRACTS is defined and properties.type is Unknown
    template < typename T >
    std::unique_ptr<DetectorInterface<T>> createDetector(const DetectorProperties<T>& properties) EXCEPT_INPUT_PARAMS;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
std::unique_ptr<MonteCarlo_NS::DetectorInterface<T>> MonteCarlo_NS::createDetector(const MonteCarlo_NS::DetectorProperties<T>& properties) EXCEPT_INPUT_PARAMS {
    switch (properties.type) {
        case DetectorType::FullAbsorber:
            return std::unique_ptr<DetectorInterface<T>>(new FullAbsorber<T>(properties));
        case DetectorType::IntegratingSphereSimple:
            return std::unique_ptr<DetectorInterface<T>>(new IntegratingSphereSimple<T>(properties));
        case DetectorType::IntegratingSphereComplex:
            return std::unique_ptr<DetectorInterface<T>>(new IntegratingSphereComplex<T>(properties));
        case DetectorType::OpticalFiber:
            return std::unique_ptr<DetectorInterface<T>>(new OpticalFiber<T>(properties));
        default:
            FAIL_ARGUMENT_CONTRACT("Invalid detector type in properties");
            return nullptr;
    }
}
