#pragma once

#include "LayerInit.h"

#include "../eigen/Eigen/Dense"
#include "../eigen/Eigen/LU"

#include <array>

#include "Eigen_NS.h"

namespace AddingDoubling_NS {
    template < typename T, size_t M >
    Matrix<T,M,M> NewE(const std::array<T,M>& v, const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> Star(const std::array<T,M>& v, const std::array<T,M>& w);

    template < typename T, size_t M >
    void Adding(const Matrix<T,M,M>& R01, const Matrix<T,M,M>& R12, Matrix<T,M,M>& R02,
                const Matrix<T,M,M>& T01, const Matrix<T,M,M>& T12, Matrix<T,M,M>& T02,
                const std::array<T,M>& v, const std::array<T,M>& w);

    template < typename T, size_t M >
    void AddingBounds(const Matrix<T,M,M>& R01, const Matrix<T,M,M>& R12, Matrix<T,M,M>& R02,
                      const Matrix<T,M,M>& T01, const Matrix<T,M,M>& T12, Matrix<T,M,M>& T02);

    template < typename T, size_t M >
    void Doubling(T a, T tau, T g, T nSlab,
                  const std::array<T,M>& v, const std::array<T,M>& w,
                  Matrix<T,M,M>& Rs, Matrix<T,M,M>& Ts);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::NewE(const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    const auto cached2aw = DoubleAW<T,M>(v, w);

    auto result = E<T,M>();
    for (int i = 0; i < m; i++)
        result(i, i) = 1 / cached2aw(i);
    return result;
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::Star(const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    const auto cached2aw = DoubleAW<T,M>(v, w);

    auto result = E<T,M>();
    for (int i = 0; i < m; i++)
        result(i, i) = cached2aw(i);
    return result;
}

template < typename T, size_t M >
void AddingDoubling_NS::Adding(const Matrix<T,M,M>& R01, const Matrix<T,M,M>& R12, Matrix<T,M,M>& R02,
                               const Matrix<T,M,M>& T01, const Matrix<T,M,M>& T12, Matrix<T,M,M>& T02,
                               const std::array<T,M>& v, const std::array<T,M>& w) {
    const auto cachedE = NewE<T,M>(v, w);
    const auto cachedS = Star<T,M>(v, w);
    const auto cached = T12 * (cachedE - R01 * cachedS * R12).inverse();
    T02 = cached * T01;
    R02 = cached * R01 * cachedS * T12 + R12;
}

template < typename T, size_t M >
void AddingDoubling_NS::AddingBounds(const Matrix<T,M,M>& R01, const Matrix<T,M,M>& R12, Matrix<T,M,M>& R02,
                                     const Matrix<T,M,M>& T01, const Matrix<T,M,M>& T12, Matrix<T,M,M>& T02) {
    /// TODO: cache - why fails tests?
    T02 = T12 * (E<T,M>() - R01 * R12).inverse() * T01;
    R02 = T12 * (E<T,M>() - R01 * R12).inverse() * R01 * T12 + R12;
}

template < typename T, size_t M >
void AddingDoubling_NS::Doubling(T a, T tau, T g, T nSlab,
                                 const std::array<T,M>& v, const std::array<T,M>& w,
                                 Matrix<T,M,M>& Rs, Matrix<T,M,M>& Ts) {
    const int n = N1<T,M>(a, tau, g, nSlab);
    Rs = RD1<T,M>(a, tau, g, nSlab, v, w);
    Ts = TD1<T,M>(a, tau, g, nSlab, v, w);
    for (int i = 0; i < n; i++) {
        const auto oldT = Ts;
        const auto oldR = Rs;
        Adding<T,M>(oldR, oldR, Rs, oldT, oldT, Ts, v, w);
    }
}
