#pragma once

#include "../Physics/Reflectance.h"

#include <assert.h>
#include <math.h>

namespace Physics_NS {
    template < typename T >
    T BugerLambert(T tau, T mediumRefractionIndex, T outerRefractionIndexTop, T outerRefractionIndexBottom);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Physics_NS::BugerLambert(T tau, T mediumRefractionIndex, T outerRefractionIndexTop, T outerRefractionIndexBottom) {
    assert(mediumRefractionIndex >= 1);
    assert(outerRefractionIndexTop >= 1);
    assert(outerRefractionIndexBottom >= 1);

    const auto reflectanceTop    = BorderReflectance<T>(mediumRefractionIndex, outerRefractionIndexTop);
    const auto reflectanceBottom = BorderReflectance<T>(mediumRefractionIndex, outerRefractionIndexBottom);
    return ((1 - reflectanceTop) * (1 - reflectanceBottom) * exp(-tau)) / (1 - reflectanceTop * reflectanceBottom * exp(-2 * tau));
}

