#pragma once

#include "Photon.h"

namespace MonteCarlo_NS {
    /// \brief Interface class for Detector
    template < typename T >
    class DetectorInterface {
    public:
        DetectorInterface() noexcept = default;

        virtual ~DetectorInterface() noexcept = default;

        /// TODO: detector writes all the data it needs in this function
        virtual void detectPhoton(const Photon<T>& photon) noexcept = 0;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/
