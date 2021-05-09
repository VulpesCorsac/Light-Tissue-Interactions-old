#pragma once

#include "Angles.h"

#include "../Utils/Utils.h"

template < typename T >
T FresnelR_AD(T n_i, T n_t, T vi) {
    return sqr((n_t * vi - n_i * Vt(n_i, n_t, vi)) / (n_t * vi + n_i * Vt(n_i, n_t, vi))) / 2 +
           sqr((n_i * vi - n_t * Vt(n_i, n_t, vi)) / (n_i * vi + n_t * Vt(n_i, n_t, vi))) / 2;
}
