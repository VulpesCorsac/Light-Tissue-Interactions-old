#pragma once

#include "Quadrature.h"

#include <algorithm>
#include <cmath>

template < typename T, size_t M >
T taus(T a, T tau, T g) {
    using namespace std;

    return (1 - a * pow(g, M)) * tau;
}

template < typename T, size_t M >
T as(T a, T g) {
    using namespace std;

    const auto gPowM = pow(g, M);
    return a * (1 - gPowM) / (1 - a * gPowM);
}

template < typename T, size_t M >
int n1(T a, T tau, T g, T n_slab) {
    using namespace std;

    Quadrature<T,M> quadrature(n_slab);
    int n = 0;
    const auto v = quadrature.getV();
    const auto treshold = taus<T,M>(a, tau, g);
    auto minElement = *min_element(begin(v), end(v));
    while (minElement * pow(2, n) < treshold)
        n++;
    return n;
}

template < typename T, size_t M >
T dtaus(T a, T tau, T g, T n_slab) {
    using namespace std;

    return taus<T,M>(a, tau, g) / pow(2, n1<T,M>(a, tau, g, n_slab));
}

template < typename T, size_t M >
T dtau(T a, T tau, T g, T n_slab) {
    using namespace std;

    return dtaus<T,M>(a, tau, g, n_slab) / (1 - a * pow(g, M));
}
