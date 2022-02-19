#pragma once

#include "Quadrature.h"

#include "../Utils/Utils.h"

#include <algorithm>
#include <math.h>

#ifdef ASSERT_INPUT_PARAMS
    #include <stdexcept>
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

template < typename T, size_t M >
T taus(T a, T tau, T g) {
    return (1 - a * pow(g, M)) * tau;
}

template < typename T, size_t M >
T as(T a, T g) {
    const auto gPowM = pow(g, M);

    #ifdef ASSERT_INPUT_PARAMS
    if (a * gPowM == 1)
        throw std::invalid_argument("a * gPowM == 1");
    #endif // ASSERT_INPUT_PARAMS

    return a * (1 - gPowM) / (1 - a * gPowM);
}

template < typename T, size_t M >
int n1(T a, T tau, T g, T nSlab) {
    Quadrature<T,M> quadrature(nSlab);
    const auto v = quadrature.getV();

    #ifdef ASSERT_INPUT_PARAMS
    if (Utils_NS::isize(v) == 0)
        throw std::invalid_argument("Utils_NS::isize(v) == 0");
    #endif // ASSERT_INPUT_PARAMS

    const auto treshold = taus<T,M>(a, tau, g);
    const auto minElement = *std::min_element(ALL_CONTAINER(v));

    int n = 0;
    while (minElement * (1 << n) < treshold)
        n++;

    return n;
}

template < typename T, size_t M >
T dtaus(T a, T tau, T g, T nSlab) {
    return taus<T,M>(a, tau, g) / (1 << n1<T,M>(a, tau, g, nSlab));
}

template < typename T, size_t M >
T dtau(T a, T tau, T g, T nSlab) {
    const auto gPowM = pow(g, M);

    #ifdef ASSERT_INPUT_PARAMS
    if (a * gPowM == 1)
        throw std::invalid_argument("a * gPowM == 1");
    #endif // ASSERT_INPUT_PARAMS

    return dtaus<T,M>(a, tau, g, nSlab) / (1 - a * gPowM);
}
