#pragma once

#include "Fresnel.h"

#include <cmath>

template < typename T >
T Rborder(T n_slab, T n_slide) {
    const auto cached1 = FresnelR<T>(n_slab, n_slide, T(1));
    const auto cached2 = FresnelR<T>(n_slide, T(1), T(1));
    const auto cached3 = cached1 * cached2;
    return (cached1 + cached2 - 2 * cached3) / (1 - cached3);
}

template < typename T >
T BugerLambert(T tau, T n_slab, T n_slide_top, T n_slide_bottom) {
    const auto Rbtop = Rborder<T>(n_slab, n_slide_top);
    const auto Rbbottom = Rborder<T>(n_slab, n_slide_bottom);
    return ((1 - Rbtop) * (1 - Rbbottom) * exp(-tau)) / (1 - Rbtop * Rbbottom * exp(-2 * tau));
}

