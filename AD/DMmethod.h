#pragma once

#include "Quadrature.h"

#include "../Utils/Utils.h"

#include <algorithm>
#include <assert.h>
#include <math.h>

template < typename T, size_t M >
T taus(T a, T tau, T g) {
    return (1 - a * pow(g, M)) * tau;
}

template < typename T, size_t M >
T as(T a, T g) {
    const auto gPowM = pow(g, M);
    return a * (1 - gPowM) / (1 - a * gPowM);
}

template < typename T, size_t M >
int n1(T a, T tau, T g, T n_slab) {
    Quadrature<T,M> quadrature(n_slab);
    const auto v = quadrature.getV();
    assert(Utils_NS::isize(v) > 0);

    const auto treshold = taus<T,M>(a, tau, g);
    const auto minElement = *std::min_element(ALL_CONTAINER(v));

    int n = 0;
    while (minElement * pow(2, n) < treshold)
        n++;

    return n;
}

template < typename T, size_t M >
T dtaus(T a, T tau, T g, T n_slab) {
    return taus<T,M>(a, tau, g) / pow(2, n1<T,M>(a, tau, g, n_slab));
}

template < typename T, size_t M >
T dtau(T a, T tau, T g, T n_slab) {
    return dtaus<T,M>(a, tau, g, n_slab) / (1 - a * pow(g, M));
}
