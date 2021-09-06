#pragma once

#include "LayerInit.h"

#include "../eigen/Eigen/Dense"
#include "../eigen/Eigen/LU"

#include <array>

using namespace Eigen;

template < typename T, size_t M >
Matrix<T,M,M> newE(const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    const auto cached2aw = twoaw<T,M>(v, w);
    Matrix<T,M,M> myE = E<T,M>();
    for (int i = 0; i < m; i++)
        myE(i, i) = 1 / cached2aw(i);
    return myE;
}

template < typename T, size_t M >
Matrix<T,M,M> star(const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    const auto cached2aw = twoaw<T,M>(v, w);
    Matrix<T,M,M> myStar = E<T,M>();
    for (int i = 0; i < m; i++)
        myStar(i, i) = cached2aw(i);
    return myStar;
}

template < typename T, size_t M >
void adding(Matrix<T,M,M> R01, Matrix<T,M,M> R12, Matrix<T,M,M>& R02, Matrix<T,M,M> T01, Matrix<T,M,M> T12, Matrix<T,M,M>& T02, std::array<T,M> v, std::array<T,M> w) {
    const auto cached = T12 * (newE<T,M>(v, w) - R01 * star<T,M>(v, w) * R12).inverse();
    T02 = cached * T01;
    R02 = cached * R01 * star<T,M>(v, w) * T12 + R12;
}

template < typename T, size_t M >
void addingBounds(const Matrix<T,M,M>& R01, const Matrix<T,M,M>& R12, Matrix<T,M,M>& R02, const Matrix<T,M,M>& T01, const Matrix<T,M,M>& T12, Matrix<T,M,M>& T02) {
    const auto cached = T12 * (E<T,M>() - R01 * R12).inverse();
    T02 = cached * T01;
    R02 = cached * R01 * T12 + R12;
}

template < typename T, size_t M >
void doubling(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w, Matrix<T,M,M>& Rs, Matrix<T,M,M>& Ts) {
    const int n = n1<T,M>(a, tau, g, n_slab);
    auto myR = Rd1<T,M>(a, tau, g, n_slab, v, w);
    auto myT = Td1<T,M>(a, tau, g, n_slab, v, w);
    for (int i = 0; i < n; i++) {
        Matrix<T,M,M> oldT = myT;
        Matrix<T,M,M> oldR = myR;
        adding<T,M>(oldR, oldR, myR, oldT, oldT, myT, v, w);
    }

    Rs = myR;
    Ts = myT;
}
