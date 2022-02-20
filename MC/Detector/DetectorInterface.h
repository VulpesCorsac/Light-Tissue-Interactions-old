#pragma once

#include "DetectorType.h"

#include "../Photon.h"
#include "../../Utils/Contracts.h"

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
        virtual void detect(const Photon<T>& photon) EXCEPT_INPUT_PARAMS;

        /// Calibrate results
        /// \param[in] totalWeights total weights for all simulated photons
        virtual void calibrate(const T& totalWeights) EXCEPT_INPUT_PARAMS;

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
void MonteCarlo_NS::DetectorInterface<T>::detect(const Photon<T>& photon) EXCEPT_INPUT_PARAMS {
    std::ignore = photon;
    FAIL_RUNTIME_CONTRACT("Detector detect was not implemented");
}

template < typename T >
void MonteCarlo_NS::DetectorInterface<T>::calibrate(const T& totalWeights) EXCEPT_INPUT_PARAMS {
    std::ignore = totalWeights;
    FAIL_RUNTIME_CONTRACT("Detector calibrate was not implemented");
}
