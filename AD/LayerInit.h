#pragma once

#include "RedistributionFunction.h"

#include <Eigen/LU>

#include <array>

template < typename T, size_t M >
Matrix<T,M,M> E() {
    return Matrix<T,M,M>::Identity();
}

int kd(int i, int j) {
    return i == j ? 1 : 0;
}

template < typename T, size_t M >
Matrix<T,M,M> B(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    const auto hpn = HPN<T,M>(v, g);
    const auto cached = as<T,M>(a, g) * dtaus<T,M>(a, tau, g, n_slab);
    Matrix<T,M,M> myB;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            myB(i, j) = cached * w[j] * hpn(i, j) / (4 * v[i]);
    return myB;
}

template < typename T, size_t M >
Matrix<T,M,M> A(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    const auto hpp = HPP<T,M>(v, g);
    const auto cached1 = dtaus<T,M>(a, tau, g, n_slab);
    const auto cached2 = cached1 * as<T,M>(a, g);
    Matrix<T,M,M> myA;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            myA(i, j) = kd(i, j) * cached1 / (2 * v[i]) - cached2 * w[j] * hpp(i, j) / (4 * v[i]);
    return myA;
}

template < typename T, size_t M >
Matrix<T,M,M> I(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w) {
    return (E<T,M>() + A<T,M>(a, tau, g, n_slab, v, w)).inverse();
}

template < typename T, size_t M >
Matrix<T,M,M> G(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w) {
    auto b = B<T,M>(a, tau, g, n_slab, v, w);
    return (E<T,M>() + A<T,M>(a, tau, g, n_slab, v, w) - b * I<T,M>(a, tau, g, n_slab, v, w) * b).inverse();
}

template < typename T, size_t M >
Matrix<T,M,M> RR(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w) {
    return 2 * G<T,M>(a, tau, g, n_slab, v, w) * B<T,M>(a, tau, g, n_slab, v, w) * I<T,M>(a, tau, g, n_slab, v, w);
}

template < typename T, size_t M >
Matrix<T,M,M> TT(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w) {
    return 2 * G<T,M>(a, tau, g, n_slab, v, w) - E<T,M>();
}

template < typename T, size_t M >
Matrix<T,M,1> twoaw(const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    Matrix<T,M,1> t;
    for (int i = 0; i < m; i++)
        t(i) = 2 * v[i] * w[i];
    return t;
}

template < typename T, size_t M >
Matrix<T,M,M> Rd1(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    const auto cachedRR = RR<T,M>(a, tau, g, n_slab, v, w);
    const auto cached2aw = twoaw<T,M>(v, w);
    Matrix<T,M,M> myRd1;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            myRd1(i, j) = cachedRR(i, j) / cached2aw(j);
    return myRd1;
}

template < typename T, size_t M >
Matrix<T,M,M> Td1(T a, T tau, T g, T Vc, const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    const auto cachedTT = TT<T,M>(a, tau, g, Vc, v, w);
    const auto cached2aw = twoaw<T,M>(v, w);
    Matrix<T,M,M> myTd1;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            myTd1(i, j) = cachedTT(i, j) / cached2aw(j);
    return myTd1;
}
