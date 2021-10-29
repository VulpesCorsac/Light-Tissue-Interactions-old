#pragma once

#include "Layer.h"

#include <vector>

namespace Physics_NS {
    template < typename T >
    class Sample {
    public:
        Sample() noexcept;

        void init() noexcept;

        void layerPushBack();
        void layerPushFront();

    public:
        std::vector<Layer<T>> layer;
        T nUpperMedium;
        T nLowerMedium;
        T totalThickness;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/
