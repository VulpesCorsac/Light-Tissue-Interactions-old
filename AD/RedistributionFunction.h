#pragma once

#include "DeltaMMethod.h"

#include "../Utils/Contracts.h"

#include "../eigen/Eigen/Dense"

#include <algorithm>
#include <array>
#include <iostream>
#include <math.h>
#include <numeric>

#include "Eigen_NS.h"

namespace AddingDoubling_NS {
    template < typename T, size_t M >
    T Chi(T g, int k);

    template < typename T, size_t M >
    Matrix<T,M,M> PP(const std::array<T,M>& a);

    template < typename T, size_t M >
    Matrix<T,M,M> PN(const std::array<T,M>& a);

    template < typename T, size_t M >
    Matrix<T,M,M> HPP(const std::array<T,M>& a, T g);

    template < typename T, size_t M >
    Matrix<T,M,M> HPN(const std::array<T,M>& a, T g);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T, size_t M >
T AddingDoubling_NS::Chi(T g, int k) {
    const auto gPowM = pow(g, M);

    CHECK_ARGUMENT_CONTRACT(gPowM != 1);

    return (pow(g, k) - gPowM) / (1 - gPowM);
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::PP(const std::array<T,M>& a) {
    using namespace std;

    const int m = M;
    Matrix<T,M,M> pp;
    for (int i = 0; i < m; i++) /// row
        for (int j = 0; j < m; j++) /// column
            pp(i, j) = legendre(i, a[j]);
    return pp;
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::PN(const std::array<T,M>& a) {
    const int m = M;
    const auto cachedPP = PP<T,M>(a);
    Matrix<T,M,M> pn;
    for (int i = 0; i < m; i++) /// row
        for (int j = 0; j < m; j++) /// column
            pn(i, j) = cachedPP(i, j) * (i % 2 == 0 ? 1 : -1);
    return pn;
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::HPP(const std::array<T,M>& a, T g) {
    const int m = M;
    Matrix<T,M,M> hpp;
    /// TODO: comparing float against zero is not a good option, some EPS is needed
    if (g == 0)
        hpp = Matrix<T,M,M>::Constant(1.0);
    else {
        const auto cachedPP = PP<T,M>(a);
        for (int i = 0; i < m; i++) /// row
            for (int j = i; j < m; j++) { /// column
                T val = 0;
                for (int k = 0; k < m; k++)
                    val += (2 * k + 1) * Chi<T,M>(g, k) * cachedPP(k, i) * cachedPP(k, j);
                hpp(i, j) = val;
            }
        for (int i = 0; i < m; i++)
            for (int j = i + 1; j < m; j++)
                hpp(j, i) = hpp(i, j);
    }
    return hpp;
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::HPN(const std::array<T,M>& a, T g) {
    const int m = M;
    Matrix<T,M,M> hpn;
    /// TODO: comparing float against zero is not a good option, some EPS is needed
    if (g == 0)
        hpn = Matrix<T,M,M>::Constant(1.0);
    else {
        const auto cachedPP = PP<T,M>(a);
        const auto cachedPN = PN<T,M>(a);
        for (int i = 0; i < m; i++) /// row
            for (int j = i; j < m; j++) { /// column
                T val = 0;
                for (int k = 0; k < m; k++)
                    val += (2 * k + 1) * Chi<T,M>(g, k) * cachedPP(k, i) * cachedPN(k, j);
                hpn(i, j) = val;
            }
        for (int i = 0; i < m; i++)
            for (int j = i + 1; j < m; j++)
                hpn(j, i) = hpn(i, j);
    }
    return hpn;
}
