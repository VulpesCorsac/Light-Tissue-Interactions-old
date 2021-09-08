#pragma once

#include "Angles.h"

#include "../Utils/Utils.h"

template < typename T >
T FresnelR_AD(T n_i, T n_t, T vi) {
    using namespace Utils_NS;

    const auto vt = Vt(n_i, n_t, vi);
    return sqr((n_t * vi - n_i * vt) / (n_t * vi + n_i * vt)) / 2 +
           sqr((n_i * vi - n_t * vt) / (n_i * vi + n_t * vt)) / 2;
}
