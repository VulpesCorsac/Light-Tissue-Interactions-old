#pragma once

#include "DMmethod.h"

#include <Eigen/Dense>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <array>
#include <numeric>

using namespace Eigen;

template < typename T, size_t M >
T chi(T g, int k) {
    using namespace std;

    const auto gPowM = pow(g, M);
    return (pow(g, k) - gPowM) / (1 - gPowM);
}

template < typename T, size_t M >
Matrix<T,M,M> PP(const std::array<T,M>& a) {
    using namespace std;

    const int m = M;
    Matrix<T,M,M> pp;
    for (int i = 0; i < m; i++) // row
        for (int j = 0; j < m; j++) // col
            pp(i, j) = legendre(i, a[j]);
    return pp;
}

template < typename T, size_t M >
Matrix<T,M,M> PN(const std::array<T,M>& a) {
    using namespace std;

    const int m = M;
    const auto cachedPP = PP<T,M>(a);
    Matrix<T,M,M> pn;
    for (int i = 0; i < m; i++) // row
        for (int j = 0; j < m; j++) // col
            pn(i, j) = cachedPP(i, j) * (i % 2 == 0 ? 1 : -1);
    return pn;
}

template < typename T, size_t M >
Matrix<T,M,M> HPP(const std::array<T,M>& a, T g) {
    const int m = M;
    Matrix<T,M,M> hpp;
    // TODO: comparing float against zero is not a good option, some EPS is needed
    if (g == 0)
        hpp = Matrix<T,M,M>::Constant(1.0);
    else {
        const auto cachedPP = PP<T,M>(a);
        for (int i = 0; i < m; i++) // row
            for (int j = i; j < m; j++) { // col
                T val = 0;
                for (int k = 0; k < m; k++)
                    val += (2 * k + 1) * chi<T,M>(g, k) * cachedPP(k, i) * cachedPP(k, j);
                hpp(i, j) = val;
            }
        for (int i = 0; i < m; i++)
            for (int j = i + 1; j < m; j++)
                hpp(j, i) = hpp(i, j);
    }
    return hpp;
}

template < typename T, size_t M >
Matrix<T,M,M> HPN(const std::array<T,M>& a, T g) {
    const int m = M;
    Matrix<T,M,M> hpn;
    // TODO: comparing float against zero is not a good option, some EPS is needed
    if (g == 0)
        hpn = Matrix<T,M,M>::Constant(1.0);
    else {
        const auto cachedPP = PP<T,M>(a);
        const auto cachedPN = PN<T,M>(a);
        for (int i = 0; i < m; i++) // row
            for (int j = i; j < m; j++) { // col
                T val = 0;
                for (int k = 0; k < m; k++)
                    val += (2 * k + 1) * chi<T,M>(g, k) * cachedPP(k, i) * cachedPN(k, j);
                hpn(i, j) = val;
            }
        for (int i = 0; i < m; i++)
            for (int j = i + 1; j < m; j++)
                hpn(j, i) = hpn(i, j);
    }
    return hpn;
}
