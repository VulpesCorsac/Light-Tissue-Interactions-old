#pragma once

#include "Quadrature.h"

#include "../Utils/Contracts.h"
#include "../Utils/Utils.h"

#include <algorithm>
#include <math.h>

namespace AddingDoubling_NS {
    template < typename T, size_t M >
    T Taus(T a, T tau, T g);

    template < typename T, size_t M >
    T As(T a, T g);

    template < typename T, size_t M >
    int N1(T a, T tau, T g, T nSlab);

    template < typename T, size_t M >
    T DTaus(T a, T tau, T g, T nSlab);

    template < typename T, size_t M >
    T DTau(T a, T tau, T g, T nSlab);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T, size_t M >
T AddingDoubling_NS::Taus(T a, T tau, T g) {
    return (1 - a * pow(g, M)) * tau;
}

template < typename T, size_t M >
T AddingDoubling_NS::As(T a, T g) {
    const auto gPowM = pow(g, M);

    CHECK_ARGUMENT_CONTRACT(a * gPowM != 1);

    return a * (1 - gPowM) / (1 - a * gPowM);
}

template < typename T, size_t M >
int AddingDoubling_NS::N1(T a, T tau, T g, T nSlab) {
    using namespace Utils_NS;
    using namespace std;

    Quadrature<T,M> quadrature(nSlab);
    const auto v = quadrature.getV();

    CHECK_ARGUMENT_CONTRACT(isize(v) > 0);

    const auto treshold = Taus<T,M>(a, tau, g);
    const auto minElement = *min_element(ALL_CONTAINER(v));

    int n = 0;
    while (minElement * (1 << n) < treshold)
        n++;

    return n;
}

template < typename T, size_t M >
T AddingDoubling_NS::DTaus(T a, T tau, T g, T nSlab) {
    return Taus<T,M>(a, tau, g) / (1 << N1<T,M>(a, tau, g, nSlab));
}

template < typename T, size_t M >
T AddingDoubling_NS::DTau(T a, T tau, T g, T nSlab) {
    const auto gPowM = pow(g, M);

    CHECK_ARGUMENT_CONTRACT(a * gPowM != 1);

    return DTaus<T,M>(a, tau, g, nSlab) / (1 - a * gPowM);
}
