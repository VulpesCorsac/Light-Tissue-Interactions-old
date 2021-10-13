#pragma once

#include "../Physics/Reflectance.h"

#include <math.h>

#ifdef ASSERT_INPUT_PARAMS
    #include <stdexcept>
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

namespace Physics_NS {
    template < typename T >
    T BugerLambert(T absorption, T mediumRefractionIndex, T outerRefractionIndexTop, T outerRefractionIndexBottom) EXCEPT_INPUT_PARAMS;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Physics_NS::BugerLambert(T absorption, T mediumRefractionIndex, T outerRefractionIndexTop, T outerRefractionIndexBottom) EXCEPT_INPUT_PARAMS {
    #ifdef ASSERT_INPUT_PARAMS
        if(mediumRefractionIndex < 1)
            throw std::invalid_argument("mediumRefractionIndex cannot be less than 1");
        if(outerRefractionIndexTop < 1)
            throw std::invalid_argument("outerRefractionIndexTop cannot be less than 1");
        if(outerRefractionIndexBottom < 1)
            throw std::invalid_argument("outerRefractionIndexBottom cannot be less than 1");
    #endif // ASSERT_INPUT_PARAMS

    const auto reflectanceTop    = BorderReflectance<T>(mediumRefractionIndex, outerRefractionIndexTop   );
    const auto reflectanceBottom = BorderReflectance<T>(mediumRefractionIndex, outerRefractionIndexBottom);
    return ((1 - reflectanceTop) * (1 - reflectanceBottom) * exp(-absorption)) / (1 - reflectanceTop * reflectanceBottom * exp(-2 * absorption));
}
