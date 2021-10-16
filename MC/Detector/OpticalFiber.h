#pragma once

#include "DetectorInterface.h"
#include "DetectorProperties.h"

namespace MonteCarlo_NS {
    /// \brief Optical fiber detector
    template < typename T >
    class OpticalFiber : public DetectorInterface<T> {
    public:
        using Base = DetectorInterface<T>;

        explicit OpticalFiber() noexcept;
        /// \param[in] properties DetectorProperties
        explicit OpticalFiber(const DetectorProperties<T>& properties) noexcept;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
MonteCarlo_NS::OpticalFiber<T>::OpticalFiber() noexcept
    : MonteCarlo_NS::OpticalFiber<T>::Base(MonteCarlo_NS::DetectorType::OpticalFiber) {
}

template < typename T >
MonteCarlo_NS::OpticalFiber<T>::OpticalFiber(const MonteCarlo_NS::DetectorProperties<T>& properties) noexcept
    : MonteCarlo_NS::OpticalFiber<T>::Base(MonteCarlo_NS::DetectorType::OpticalFiber) {
}
