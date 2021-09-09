#pragma once

#include "Angles.h"

#include "../Utils/Utils.h"

#include <assert.h>
#include <math.h>

namespace Physics_NS {
    template < typename T >
    T FresnelReflectance(T incidenceRefractionIndex, T transmittanceRefractionIndex, T incidenceCos);

    template < typename T >
    T BorderReflectance(T mediumRefractionIndex, T outerRefractionIndex);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Physics_NS::FresnelReflectance(T incidenceRefractionIndex, T transmittanceRefractionIndex, T incidenceCos) {
    using namespace Utils_NS;

    assert(incidenceRefractionIndex >= 1);
    assert(transmittanceRefractionIndex >= 1);
    assert(incidenceCos <= +1);
    assert(incidenceCos >= -1);

    const T transmittanceCos = TransmittanceCos(incidenceRefractionIndex, transmittanceRefractionIndex, incidenceCos);
    const T cached1 = (transmittanceRefractionIndex * incidenceCos - incidenceRefractionIndex * transmittanceCos) / (transmittanceRefractionIndex * incidenceCos + incidenceRefractionIndex * transmittanceCos);
    const T cached2 = (incidenceRefractionIndex * incidenceCos - transmittanceRefractionIndex * transmittanceCos) / (incidenceRefractionIndex * incidenceCos + transmittanceRefractionIndex * transmittanceCos);
    return 0.5 * (sqr(cached1) + sqr(cached2));
}

template < typename T >
T Physics_NS::BorderReflectance(T mediumRefractionIndex, T outerRefractionIndex) {
    assert(mediumRefractionIndex >= 1);
    assert(outerRefractionIndex >= 1);

    const auto cached1 = FresnelReflectance<T>(mediumRefractionIndex, outerRefractionIndex, static_cast<T>(1));
    const auto cached2 = FresnelReflectance<T>(outerRefractionIndex, static_cast<T>(1), static_cast<T>(1));
    const auto cached3 = cached1 * cached2;
    return (cached1 + cached2 - 2 * cached3) / (1 - cached3);
}
