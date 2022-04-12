#pragma once

#include "LayerProperties.h"
#include "Quadrature.h"
#include "../MC/Medium.h"

#include "../Utils/Contracts.h"
#include "../Utils/Utils.h"

#include <algorithm>
#include <math.h>

namespace AddingDoubling_NS {
    template < typename T, size_t M >
    T Taus(T a, T tau, T g);

    template < typename T, size_t M >
    T Taus(const Medium<T>& layer);

    template < typename T, size_t M >
    T As(T a, T g);

    template < typename T, size_t M >
    T As(const Medium<T>& layer);

    template < typename T, size_t M >
    int N1(const Medium<T>& layer);

    template < typename T, size_t M >
    T DTaus(const Medium<T>& layer);

    template < typename T, size_t M >
    T DTau(const Medium<T>& layer);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T, size_t M >
T AddingDoubling_NS::Taus(T a, T tau, T g) {
    return (1 - a * pow(g, M)) * tau;
}

template < typename T, size_t M >
T AddingDoubling_NS::Taus(const Medium<T>& layer) {
    return Taus<T,M>(layer.getA(), layer.getTau(), layer.getG());
}

template < typename T, size_t M >
T AddingDoubling_NS::As(T a, T g) {
    const auto gPowM = pow(g, M);

    CHECK_ARGUMENT_CONTRACT(a * gPowM != 1);

    return a * (1 - gPowM) / (1 - a * gPowM);
}

template < typename T, size_t M >
T AddingDoubling_NS::As(const Medium<T>& layer) {
    return As<T,M>(layer.getA(), layer.getG());
}

template < typename T, size_t M >
int AddingDoubling_NS::N1(const Medium<T>& layer) {
    using namespace Utils_NS;
    using namespace std;

    Quadrature<T,M> quadrature(layer.getN());
    const auto v = quadrature.getV();

    CHECK_ARGUMENT_CONTRACT(isize(v) > 0);

    const auto treshold = Taus<T,M>(layer);
    const auto minElement = *min_element(ALL_CONTAINER(v));

    int n = 0;
    while (minElement * (1 << n) < treshold)
        n++;

    return n;
}

template < typename T, size_t M >
T AddingDoubling_NS::DTaus(const Medium<T>& layer) {
    return Taus<T,M>(layer) / (1 << N1<T,M>(layer));
}

template < typename T, size_t M >
T AddingDoubling_NS::DTau(const Medium<T>& layer) {
    const auto gPowM = pow(layer.getG(), M);

    CHECK_ARGUMENT_CONTRACT(layer.getA() * gPowM != 1);

    return DTaus<T,M>(layer) / (1 - layer.getA() * gPowM);
}
