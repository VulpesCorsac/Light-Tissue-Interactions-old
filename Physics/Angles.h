#pragma once

#include "../Math/Basic.h"

#include <assert.h>
#include <math.h>

namespace Physics_NS {
    template < typename T >
    T TransmittanceCos(T incidenceRefractionIndex, T transmittanceRefractionIndex, T incidenceCos);

    template < typename T >
    T CriticalCos(T incidenceRefractionIndex, T transmittanceRefractionIndex);

    template < typename T >
    T CriticalCos(T transmittanceRefractionIndex);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Physics_NS::TransmittanceCos(T incidenceRefractionIndex, T transmittanceRefractionIndex, T incidenceCos) {
    assert(incidenceRefractionIndex >= 1);
    assert(transmittanceRefractionIndex >= 1);
    assert(incidenceCos <= +1);
    assert(incidenceCos >= -1);

    const T cached = 1 - Math_NS::sqr(incidenceRefractionIndex / transmittanceRefractionIndex) * (1 - Math_NS::sqr(incidenceCos));
    if (cached < 0)
        return 0;

    return incidenceCos > 0 ? sqrt(cached) : -sqrt(cached);
}

template < typename T >
T Physics_NS::CriticalCos(T incidenceRefractionIndex, T transmittanceRefractionIndex) {
    assert(incidenceRefractionIndex >= 1);
    assert(transmittanceRefractionIndex >= 1);

    return cos(asin(std::min(incidenceRefractionIndex, transmittanceRefractionIndex) /
                    std::max(incidenceRefractionIndex, transmittanceRefractionIndex)));
}

template < typename T >
T Physics_NS::CriticalCos(T transmittanceRefractionIndex) {
    return CriticalCos(static_cast<T>(1), transmittanceRefractionIndex);
}
