#pragma once

#include "../Utils/Utils.h"

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
    using namespace std;
    using namespace Utils_NS;

    assert(incidenceRefractionIndex >= 1);
    assert(transmittanceRefractionIndex >= 1);
    assert(incidenceCos <= +1);
    assert(incidenceCos >= -1);

    const T cached = 1 - sqr(incidenceRefractionIndex / transmittanceRefractionIndex) * (1 - sqr(incidenceCos));
    if (cached < 0)
        return 0;

    return incidenceCos > 0 ? sqrt(cached) : -sqrt(cached);
}

template < typename T >
T Physics_NS::CriticalCos(T incidenceRefractionIndex, T transmittanceRefractionIndex) {
    using namespace std;

    assert(incidenceRefractionIndex >= 1);
    assert(transmittanceRefractionIndex >= 1);

    return cos(asin(min(incidenceRefractionIndex, transmittanceRefractionIndex) / max(incidenceRefractionIndex, transmittanceRefractionIndex)));
}

template < typename T >
T Physics_NS::CriticalCos(T transmittanceRefractionIndex) {
    return CriticalCos(static_cast<T>(1), transmittanceRefractionIndex);
}
