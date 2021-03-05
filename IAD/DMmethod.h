#ifndef DMMETHOD_H
#define DMMETHOD_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

template <class T, size_t M>
T taus (T& a, T& tau, T& g) {
    return ((1 - a*pow(g, M))*tau);
}

template <class T, size_t M>
T as (T& a, T& g) {
    return (a*(1 - pow(g, M))/(1 - a*pow(g, M)));
}

template <class T, size_t M>
int n1 (T& a, T& tau, T& g, T& Vc) {
    Quadrature<T, M> quadrature(Vc);
    int n = 0;
    array<T, M> v = quadrature.getV();
    while (taus<T, M>(a, tau, g) > (*min_element(v.begin(), v.end())) * pow(2.0, n)) {
        n++;
    }
    return n;
}

template <class T, size_t M>
T dtaus (T& a, T& tau, T& g, T& Vc) {
    return (taus<T, M>(a, tau, g)/pow(2, n1<T, M>(a, tau, g, Vc)));
}

template <class T, size_t M>
T dtau (T& a, T& tau, T& g, T& Vc) {
    return (dtaus<T, M>(a, tau, g, Vc)/(1 - a*pow(g, M)));
}

#endif // DMMETHOD_H
