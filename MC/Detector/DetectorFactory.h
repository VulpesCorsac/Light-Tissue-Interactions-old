#pragma once

#include "DetectorInterface.h"
#include "DetectorProperties.h"
#include "DetectorType.h"
#include "FullAbsorber.h"
#include "IntegratingSphereComplex.h"
#include "IntegratingSphereSimple.h"
#include "OpticalFiber.h"

#include <memory>

#ifdef ASSERT_INPUT_PARAMS
    #include <stdexcept>
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

namespace MonteCarlo_NS {
    /// \brief factory function to create detector of needed type based on properties
    /// \param[in] properties DetectorProperties
    /// \return detector of needed type based on properties
    /// \throw std::invalid_argument If ASSERT_INPUT_PARAMS is defined and properties.type is Unknown
    template < typename T >
    std::unique_ptr<DetectorInterface<T>> createDetector(const DetectorProperties<T>& properties) EXCEPT_INPUT_PARAMS;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
std::unique_ptr<MonteCarlo_NS::DetectorInterface<T>> MonteCarlo_NS::createDetector(const MonteCarlo_NS::DetectorProperties<T>& properties) EXCEPT_INPUT_PARAMS {
    switch (properties.type) {
        case MonteCarlo_NS::DetectorType::FullAbsorber:
            return std::unique_ptr<MonteCarlo_NS::DetectorInterface<T>>(new MonteCarlo_NS::FullAbsorber<T>(properties));
        case MonteCarlo_NS::DetectorType::IntegratingSphereSimple:
            return std::unique_ptr<MonteCarlo_NS::DetectorInterface<T>>(new MonteCarlo_NS::IntegratingSphereSimple<T>(properties));
        case MonteCarlo_NS::DetectorType::IntegratingSphereComplex:
            return std::unique_ptr<MonteCarlo_NS::DetectorInterface<T>>(new MonteCarlo_NS::IntegratingSphereComplex<T>(properties));
        case MonteCarlo_NS::DetectorType::OpticalFiber:
            return std::unique_ptr<MonteCarlo_NS::DetectorInterface<T>>(new MonteCarlo_NS::OpticalFiber<T>(properties));
        default:
            #ifdef ASSERT_INPUT_PARAMS
                throw std::invalid_argument("Invalid detector type in properties");
            #endif // ASSERT_INPUT_PARAMS
            return nullptr;
    }
}
