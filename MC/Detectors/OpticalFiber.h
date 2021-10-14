#pragma once

#include "DetectorInterface.h"

namespace MonteCarlo_NS {
    template < typename T >
    class OpticalFiber : public DetectorInterface<T> {
    public:
        /// just not to be pure virtual
        void detectPhoton(const Photon<T>& photon) noexcept override {
            std::ignore = photon;
            return;
        }
    };
}

/******************
 * IMPLEMENTATION *
 ******************/
