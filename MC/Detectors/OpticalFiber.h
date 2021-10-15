#pragma once

#include "DetectorInterface.h"

namespace MonteCarlo_NS {
    template < typename T >
    class OpticalFiber : public DetectorInterface<T> {
    public:
        using Base = DetectorInterface<T>;

    };
}

/******************
 * IMPLEMENTATION *
 ******************/
