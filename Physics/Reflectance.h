#pragma once

#include "Angles.h"

#include "../Utils/Utils.h"

#include <math.h>

#ifdef ASSERT_INPUT_PARAMS
    #include <stdexcept>
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

namespace Physics_NS {
    template < typename T >
    T FresnelReflectance(T incidenceRefractionIndex, T transmittanceRefractionIndex, T incidenceCos) EXCEPT_INPUT_PARAMS;

    template < typename T >
    T BorderReflectance(T mediumRefractionIndex, T outerRefractionIndex) EXCEPT_INPUT_PARAMS;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
T Physics_NS::FresnelReflectance(T incidenceRefractionIndex, T transmittanceRefractionIndex, T incidenceCos) EXCEPT_INPUT_PARAMS {
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

    const auto transmittanceCos = TransmittanceCos(incidenceRefractionIndex, transmittanceRefractionIndex, incidenceCos);
    const auto cached1 = (transmittanceRefractionIndex * incidenceCos -     incidenceRefractionIndex * transmittanceCos) /
                         (transmittanceRefractionIndex * incidenceCos +     incidenceRefractionIndex * transmittanceCos);
    const auto cached2 = (    incidenceRefractionIndex * incidenceCos - transmittanceRefractionIndex * transmittanceCos) /
                         (    incidenceRefractionIndex * incidenceCos + transmittanceRefractionIndex * transmittanceCos);
    return (Math_NS::sqr(cached1) + Math_NS::sqr(cached2)) / 2;
}

template < typename T >
T Physics_NS::BorderReflectance(T mediumRefractionIndex, T outerRefractionIndex) EXCEPT_INPUT_PARAMS {
    #ifdef ASSERT_INPUT_PARAMS
        if(mediumRefractionIndex < 1)
            throw std::invalid_argument("mediumRefractionIndex cannot be less than 1");
        if(outerRefractionIndex < 1)
            throw std::invalid_argument("outerRefractionIndex cannot be less than 1");
    #endif // ASSERT_INPUT_PARAMS

    const auto cached1 = FresnelReflectance(mediumRefractionIndex, outerRefractionIndex, static_cast<T>(1));
    const auto cached2 = FresnelReflectance(outerRefractionIndex , static_cast<T>(1)   , static_cast<T>(1));
    const auto cached3 = cached1 * cached2;
    return (cached1 + cached2 - 2 * cached3) / (1 - cached3);
}
