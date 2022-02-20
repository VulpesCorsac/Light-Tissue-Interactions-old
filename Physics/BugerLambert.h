#pragma once

#include "../Physics/Reflectance.h"
#include "../Utils/Contracts.h"

#include <math.h>

namespace Physics_NS {
    template < typename T >
    T BugerLambert(T absorption, T mediumRefractionIndex, T outerRefractionIndexTop, T outerRefractionIndexBottom) EXCEPT_INPUT_PARAMS;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Physics_NS::BugerLambert(T absorption, T mediumRefractionIndex, T outerRefractionIndexTop, T outerRefractionIndexBottom) EXCEPT_INPUT_PARAMS {
    CHECK_ARGUMENT_CONTRACT(mediumRefractionIndex >= 1);
    CHECK_ARGUMENT_CONTRACT(outerRefractionIndexTop >= 1);
    CHECK_ARGUMENT_CONTRACT(outerRefractionIndexBottom >= 1);

    const auto reflectanceTop    = BorderReflectance<T>(mediumRefractionIndex, outerRefractionIndexTop   );
    const auto reflectanceBottom = BorderReflectance<T>(mediumRefractionIndex, outerRefractionIndexBottom);

    CHECK_RUNTIME_CONTRACT(1 - reflectanceTop * reflectanceBottom * exp(-2 * absorption) != 0);

    return ((1 - reflectanceTop) * (1 - reflectanceBottom) * exp(-absorption)) / (1 - reflectanceTop * reflectanceBottom * exp(-2 * absorption));
}
