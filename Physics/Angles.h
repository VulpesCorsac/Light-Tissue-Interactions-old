#pragma once

#include "../Math/Basic.h"

#include <math.h>

#ifdef ASSERT_INPUT_PARAMS
    #include <stdexcept>
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

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
    #ifdef ASSERT_INPUT_PARAMS
        if(incidenceRefractionIndex < 1)
            throw std::invalid_argument("incidenceRefractionIndex cannot be less than 1");
        if(transmittanceRefractionIndex < 1)
            throw std::invalid_argument("transmittanceRefractionIndex cannot be less than 1");
        if(incidenceCos > +1)
            throw std::invalid_argument("incidenceCos cannot be greater than +1");
        if(incidenceCos < -1)
            throw std::invalid_argument("incidenceCos cannot be less than -1");
    #endif // ASSERT_INPUT_PARAMS

    const T cached = 1 - Math_NS::sqr(incidenceRefractionIndex / transmittanceRefractionIndex) * (1 - Math_NS::sqr(incidenceCos));
    if (cached < 0)
        return 0;

    return incidenceCos > 0 ? sqrt(cached) : -sqrt(cached);
}

template < typename T >
T Physics_NS::CriticalCos(T incidenceRefractionIndex, T transmittanceRefractionIndex) EXCEPT_INPUT_PARAMS {
    #ifdef ASSERT_INPUT_PARAMS
        if(incidenceRefractionIndex < 1)
            throw std::invalid_argument("incidenceRefractionIndex cannot be less than 1");
        if(transmittanceRefractionIndex < 1)
            throw std::invalid_argument("transmittanceRefractionIndex cannot be less than 1");
    #endif // ASSERT_INPUT_PARAMS

    return cos(asin(std::min(incidenceRefractionIndex, transmittanceRefractionIndex) /
                    std::max(incidenceRefractionIndex, transmittanceRefractionIndex)));
}

template < typename T >
T Physics_NS::CriticalCos(T transmittanceRefractionIndex) EXCEPT_INPUT_PARAMS {
    return CriticalCos(static_cast<T>(1), transmittanceRefractionIndex);
}
