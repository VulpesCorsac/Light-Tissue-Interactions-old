#pragma once

#include "DetectorType.h"

#include "../Photon.h"

#include <memory>

namespace MonteCarlo_NS {
    /// \brief Interface class for Detector
    template < typename T >
    class DetectorInterface {
    public:
        /// \param[in] type is used to set in derived classes
        DetectorInterface(const DetectorType& type = DetectorType::Unknown) noexcept;

        /// virtual destructor for interface
        virtual ~DetectorInterface() noexcept = default;

        /// Accumulate photon in the detector if needed
        /// \param[in] photon Photon that should be detected
        virtual void detect(const Photon<T>& photon);

        /// Calibrate results
        /// \param[in] totalWeights total weights for all simulated photons
        virtual void calibrate(const T& totalWeights);

    public:
        DetectorType type; ///< detector type
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
MonteCarlo_NS::DetectorInterface<T>::DetectorInterface(const DetectorType& type) noexcept
    : type(type) {
}

template < typename T >
void MonteCarlo_NS::DetectorInterface<T>::detect(const Photon<T>& photon) {
    std::ignore = photon;
    throw std::runtime_error("Detector detect was not implemented");
}

template < typename T >
void MonteCarlo_NS::DetectorInterface<T>::calibrate(const T& totalWeights) {
    std::ignore = totalWeights;
    throw std::runtime_error("Detector calibrate was not implemented");
}
