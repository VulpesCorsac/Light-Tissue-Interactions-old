#pragma once

#include "DetectorInterface.h"
#include "DetectorProperties.h"

namespace MonteCarlo_NS {
    /// \brief Complex integrating sphere
    template < typename T >
    class IntegratingSphereComplex : public DetectorInterface<T> {
    public:
        using Base = DetectorInterface<T>;

        explicit IntegratingSphereComplex() noexcept;
        /// \param[in] properties DetectorProperties
        explicit IntegratingSphereComplex(const DetectorProperties<T>& properties) noexcept;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
MonteCarlo_NS::IntegratingSphereComplex<T>::IntegratingSphereComplex() noexcept
    : Base(DetectorType::IntegratingSphereComplex) {
}

template < typename T >
MonteCarlo_NS::IntegratingSphereComplex<T>::IntegratingSphereComplex(const MonteCarlo_NS::DetectorProperties<T>& properties) noexcept
    : Base(DetectorType::IntegratingSphereComplex) {
}
