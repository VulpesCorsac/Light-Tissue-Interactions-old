#pragma once

#include "DetectorInterface.h"
#include "DetectorProperties.h"

#include <iostream>

namespace MonteCarlo_NS {
    template < typename T >
    class FullAbsorber : public DetectorInterface<T> {
    public:
        using Base = DetectorInterface<T>;

        /// \param[in] collimatedCosine cosine for evaluating as collimated
        explicit FullAbsorber(const T& collimatedCosine = 1) noexcept;
        /// \param[in] properties DetectorProperties
        explicit FullAbsorber(const DetectorProperties<T>& properties) noexcept;

        void detect(const Photon<T>& photon) override;
        void calibrate(const T& totalWeights) override;

    public:
        const T collimatedCosine; ///< minimal absolute cosine to detect photon as collimated

    public:
        T collimatedAbsorbed = 0; ///< total absorbed collimated weights
        T diffusiveAbsorbed  = 0; ///< total absorbed diffusive weights
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
MonteCarlo_NS::FullAbsorber<T>::FullAbsorber(const T& collimatedCosine) noexcept
    : MonteCarlo_NS::FullAbsorber<T>::Base(DetectorType::FullAbsorber)
    , collimatedCosine(collimatedCosine) {
}

template < typename T >
MonteCarlo_NS::FullAbsorber<T>::FullAbsorber(const MonteCarlo_NS::DetectorProperties<T>& properties) noexcept
    : MonteCarlo_NS::FullAbsorber<T>::Base(MonteCarlo_NS::DetectorType::FullAbsorber)
    , collimatedCosine(properties.collimatedCosine.has_value() ? properties.collimatedCosine.value() : 1) {
}

template < typename T >
void MonteCarlo_NS::FullAbsorber<T>::detect(const Photon<T>& photon) {
    if (std::abs(photon.direction.z) < collimatedCosine)
        diffusiveAbsorbed += photon.weight;
    else
        collimatedAbsorbed += photon.weight;
}

template < typename T >
void MonteCarlo_NS::FullAbsorber<T>::calibrate(const T& totalWeights) {
    collimatedAbsorbed /= totalWeights;
    diffusiveAbsorbed  /= totalWeights;
}
