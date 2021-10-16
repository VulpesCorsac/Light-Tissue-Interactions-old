#pragma once

#include "DetectorInterface.h"
#include "DetectorProperties.h"

namespace MonteCarlo_NS {
    /// \brief Simple integrating sphere
    template < typename T >
    class IntegratingSphereSimple : public DetectorInterface<T> {
    public:
        using Base = DetectorInterface<T>;

        explicit IntegratingSphereSimple() noexcept;
        /// \param[in] properties DetectorProperties
        explicit IntegratingSphereSimple(const DetectorProperties<T>& properties) noexcept;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
MonteCarlo_NS::IntegratingSphereSimple<T>::IntegratingSphereSimple() noexcept
    : MonteCarlo_NS::IntegratingSphereSimple<T>::Base(MonteCarlo_NS::DetectorType::IntegratingSphereSimple) {
}

template < typename T >
MonteCarlo_NS::IntegratingSphereSimple<T>::IntegratingSphereSimple(const MonteCarlo_NS::DetectorProperties<T>& properties) noexcept
    : MonteCarlo_NS::IntegratingSphereSimple<T>::Base(MonteCarlo_NS::DetectorType::IntegratingSphereSimple) {
}
