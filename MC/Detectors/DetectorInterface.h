#pragma once

#include "DetectorTypes.h"

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
