#ifndef FRESNEL_H
#define FRESNEL_H

#include <complex>

#include "Angles.h"

template <class T>
T FresnelR (T n_i, T n_t, T vi) {
    return (pow((n_t * vi - n_i * Vt(n_i, n_t, vi))/(n_t * vi + n_i * Vt(n_i, n_t, vi)), 2)/2 + pow((n_i * vi - n_t * Vt(n_i, n_t, vi))/(n_i * vi + n_t * Vt(n_i, n_t, vi)), 2)/2);
}

#endif // FRESNEL_H
