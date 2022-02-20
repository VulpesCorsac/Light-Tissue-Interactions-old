#pragma once

#include "Angles.h"

#include "../Utils/Contracts.h"
#include "../Utils/Utils.h"

#include <math.h>

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
    using namespace Math_NS;

    CHECK_ARGUMENT_CONTRACT(incidenceRefractionIndex     >= 1);
    CHECK_ARGUMENT_CONTRACT(transmittanceRefractionIndex >= 1);
    CHECK_ARGUMENT_CONTRACT(incidenceCos <= +1);
    CHECK_ARGUMENT_CONTRACT(incidenceCos >= -1);

    const auto transmittanceCos = TransmittanceCos(incidenceRefractionIndex, transmittanceRefractionIndex, incidenceCos);

    CHECK_RUNTIME_CONTRACT(transmittanceRefractionIndex * incidenceCos +     incidenceRefractionIndex * transmittanceCos != 0);
    CHECK_RUNTIME_CONTRACT(    incidenceRefractionIndex * incidenceCos + transmittanceRefractionIndex * transmittanceCos != 0);

    const auto cached1 = (transmittanceRefractionIndex * incidenceCos -     incidenceRefractionIndex * transmittanceCos) /
                         (transmittanceRefractionIndex * incidenceCos +     incidenceRefractionIndex * transmittanceCos);
    const auto cached2 = (    incidenceRefractionIndex * incidenceCos - transmittanceRefractionIndex * transmittanceCos) /
                         (    incidenceRefractionIndex * incidenceCos + transmittanceRefractionIndex * transmittanceCos);
    return (sqr(cached1) + sqr(cached2)) / 2;
}

template < typename T >
T Physics_NS::BorderReflectance(T mediumRefractionIndex, T outerRefractionIndex) EXCEPT_INPUT_PARAMS {
    CHECK_ARGUMENT_CONTRACT(mediumRefractionIndex >= 1);
    CHECK_ARGUMENT_CONTRACT(outerRefractionIndex  >= 1);

    const auto cached1 = FresnelReflectance(mediumRefractionIndex, outerRefractionIndex, static_cast<T>(1));
    const auto cached2 = FresnelReflectance(outerRefractionIndex , static_cast<T>(1)   , static_cast<T>(1));
    const auto cached3 = cached1 * cached2;

    CHECK_RUNTIME_CONTRACT(cached3 != 1);

    return (cached1 + cached2 - 2 * cached3) / (1 - cached3);
}
