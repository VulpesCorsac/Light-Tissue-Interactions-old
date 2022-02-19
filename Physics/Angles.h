#pragma once

#include "../Math/Basic.h"
#include "../Utils/Contracts.h"

#include <math.h>

#include <iostream>

namespace Physics_NS {
    template < typename T >
    T TransmittanceCos(T incidenceRefractionIndex, T transmittanceRefractionIndex, T incidenceCos) EXCEPT_INPUT_PARAMS;

    template < typename T >
    T CriticalCos(T incidenceRefractionIndex, T transmittanceRefractionIndex) EXCEPT_INPUT_PARAMS;

    template < typename T >
    T CriticalCos(T transmittanceRefractionIndex) EXCEPT_INPUT_PARAMS;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Physics_NS::TransmittanceCos(T incidenceRefractionIndex, T transmittanceRefractionIndex, T incidenceCos) EXCEPT_INPUT_PARAMS {
    using namespace Math_NS;
    using namespace std;

    CHECK_CONTRACT(transmittanceRefractionIndex >= 1, invalid_argument)
    CHECK_CONTRACT(incidenceRefractionIndex     >= 1, invalid_argument)
    CHECK_CONTRACT(incidenceCos <= +1               , invalid_argument)
    CHECK_CONTRACT(incidenceCos >= -1               , invalid_argument)

    const T cached = 1 - sqr(incidenceRefractionIndex / transmittanceRefractionIndex) * (1 - sqr(incidenceCos));
    if (cached < 0)
        return 0;

    return incidenceCos > 0 ? sqrt(cached) : -sqrt(cached);
}

template < typename T >
T Physics_NS::CriticalCos(T incidenceRefractionIndex, T transmittanceRefractionIndex) EXCEPT_INPUT_PARAMS {
    using namespace std;

    CHECK_CONTRACT(incidenceRefractionIndex     >= 1, invalid_argument)
    CHECK_CONTRACT(transmittanceRefractionIndex >= 1, invalid_argument)

    return cos(asin(min(incidenceRefractionIndex, transmittanceRefractionIndex) /
                    max(incidenceRefractionIndex, transmittanceRefractionIndex)));
}

template < typename T >
T Physics_NS::CriticalCos(T transmittanceRefractionIndex) EXCEPT_INPUT_PARAMS {
    return CriticalCos(static_cast<T>(1), transmittanceRefractionIndex);
}
