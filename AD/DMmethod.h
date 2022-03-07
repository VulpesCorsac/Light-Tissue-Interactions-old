#pragma once

#include "Quadrature.h"

#include "../Utils/Contracts.h"
#include "../Utils/Utils.h"

#include <algorithm>
#include <math.h>

template < typename T, size_t M >
T taus(T a, T tau, T g) {
    return (1 - a * pow(g, M)) * tau;
}

template < typename T, size_t M >
T as(T a, T g) {
    const auto gPowM = pow(g, M);

    CHECK_ARGUMENT_CONTRACT(a * gPowM != 1);

    return a * (1 - gPowM) / (1 - a * gPowM);
}

template < typename T, size_t M >
int n1(T a, T tau, T g, T nSlab) {
    using namespace Utils_NS;
    using namespace std;

    Quadrature<T,M> quadrature(nSlab);
    const auto v = quadrature.getV();

    CHECK_ARGUMENT_CONTRACT(isize(v) > 0);

    const auto treshold = taus<T,M>(a, tau, g);
    const auto minElement = *min_element(ALL_CONTAINER(v));

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

    CHECK_ARGUMENT_CONTRACT(a * gPowM != 1);

    return dtaus<T,M>(a, tau, g, nSlab) / (1 - a * gPowM);
}
