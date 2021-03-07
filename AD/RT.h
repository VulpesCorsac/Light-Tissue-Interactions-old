#pragma once

#include "Fresnel.h"
#include "AddingDoubling.h"

#include "../Utils/Utils.h"

#include <Eigen/Dense>

#include <array>

using namespace Eigen;

template < typename T, size_t M >
void RTslab(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w, Matrix<T,M,M>& Rslab, Matrix<T,M,M>& Tslab){
    doubling<T,M>(a, tau, g, n_slab, v, w, Rslab, Tslab);
}

/*template < typename T, size_t M >
Matrix<T,M,M> Rslab(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w) {
    Matrix<T,M,M> myRslab;
    Matrix<T,M,M> myTslab;
    doubling<T,M>(a, tau, g, n_slab, v, w, myRslab, myTslab);
    return myRslab;
}

template < typename T, size_t M >
Matrix<T,M,M> Tslab(T a, T tau, T g, T n_slab, const std::array<T,M>& v, const std::array<T,M>& w) {
    Matrix<T,M,M> myRslab;
    Matrix<T,M,M> myTslab;
    doubling<T,M>(a, tau, g, n_slab, v, w, myRslab, myTslab);
    return myTslab;
}*/

template < typename T, size_t M >
Matrix<T,M,M> Rbound(T a, T tau, T g, T n_slab, T n_slide, const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    Matrix<T,M,M> myRb = E<T,M>();
    for (int i = 0; i < m; i++) {
        const auto cached1 = FresnelR(n_slide, static_cast<T>(1), Vt(n_slab, n_slide, v[i]));
        const auto cached2 = FresnelR(n_slab, n_slide, v[i]);
        const auto cached3 = cached1 * cached2;
        myRb(i, i) = std::real(twoaw<T,M>(v, w)(i) * (cached2 + cached1 - 2 * cached3)) / (1 - cached3);
    }
    return myRb;
}

template < typename T, size_t M >
Matrix<T,M,M> Tbound(T a, T tau, T g, T n_slab, T n_slide, const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    Matrix<T,M,M> myTb = E<T,M>();
    for (int i = 0; i < m; i++) {
        const auto cached1 = FresnelR(n_slide, static_cast<T>(1), Vt(n_slab, n_slide, v[i]));
        const auto cached2 = FresnelR(n_slab, n_slide, v[i]);
        const auto cached3 = cached1 * cached2;
        myTb(i, i) = std::real(1 - (cached2 + cached1 - 2 * cached3) / (1 - cached3));
    }
    return myTb;
}


template < typename T, size_t M >
void RTtotal(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom, const std::array<T,M>& v, const std::array<T,M>& w, Matrix<T,M,M>& Rtotal, Matrix<T,M,M>& Ttotal) {
    const int m = M;
    Matrix<T,M,M> T02, R20, T03, R30, myRslab, myTslab, R30mod;
    RTslab<T,M>(a, tau, g, n_slab, v, w, myRslab, myTslab);
    addingBounds<T,M>(Rbound<T,M>(a, tau, g, n_slab, n_slide_top, v, w), myRslab, R20, Tbound<T,M>(a, tau, g, n_slab, n_slide_top, v, w), myTslab, T02);
    addingBounds<T,M>(R20, Rbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w), R30, T02, Tbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w), Ttotal);
    R30mod = R30 - Rbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w);
    for (int i = 0; i < m; i++)
        R30mod(i, i) += Rbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w)(i, i) / sqr(twoaw<T,M>(v, w)(i));
    Rtotal = R30mod;
}

/*

template < typename T, size_t M >
Matrix<T,M,M> Ttotal(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom, const std::array<T,M>& v, const std::array<T,M>& w) {
    Matrix<T,M,M> T02t, R20t, T03t, R30t;
    addingBounds<T,M>(Rbound<T,M>(a, tau, g, n_slab, n_slide_top, v, w), Rslab<T,M>(a, tau, g, n_slab, v, w), R20t, Tbound<T,M>(a, tau, g, n_slab, n_slide_top, v, w), Tslab<T,M>(a, tau, g, n_slab, v, w), T02t);
    addingBounds<T,M>(R20t, Rbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w), R30t, T02t, Tbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w), T03t);
    return T03t;
}

template < typename T, size_t M >
Matrix<T,M,M> Rtotal(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom, const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    Matrix<T,M,M> T02r, R20r, T03r, R30r, R30mod;
    addingBounds<T,M>(Rbound<T,M>(a, tau, g, n_slab, n_slide_top, v, w), Rslab<T,M>(a, tau, g, n_slab, v, w), R20r, Tbound<T,M>(a, tau, g, n_slab, n_slide_top, v, w), Tslab<T,M>(a, tau, g, n_slab, v, w), T02r);
    addingBounds<T,M>(R20r, Rbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w), R30r, T02r, Tbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w), T03r);
    R30mod = R30r - Rbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w);
    for (int i = 0; i < m; i++)
        R30mod(i, i) += Rbound<T,M>(a, tau, g, n_slab, n_slide_bottom, v, w)(i, i) / sqr(twoaw<T,M>(v, w)(i));
    return R30mod;
}

//*/

template < typename T, size_t M >
void RTs(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom, const std::array<T,M>& v, const std::array<T,M>& w, T& Rs, T& Ts) {
    const int m = M;
    Ts = Rs = 0;
    Matrix<T,M,M> Ttot, Rtot;
    RTtotal<T,M>(a, tau, g, n_slab, n_slide_top, n_slide_bottom, v, w, Rtot, Ttot);
    for (int i = 0; i < m; i++) {
        Ts += 2 * v[i] * w[i] * Ttot(i, m - 1);
        Rs += 2 * v[i] * w[i] * Rtot(i, m - 1);
    }
}

/*

template < typename T, size_t M >
T Ts(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom, const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    T myTs = 0;
    Matrix<T,M,M> Ttot = Ttotal<T,M> (a, tau, g, n_slab, n_slide_top, n_slide_bottom, v, w);
    for (int i = 0; i < m; i++)
        myTs += 2 * v[i] * w[i] * Ttot(i, m - 1);
    return myTs;
}

template < typename T, size_t M >
T Rs(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom, const std::array<T,M>& v, const std::array<T,M>& w) {
    const int m = M;
    T myRs = 0;
    Matrix<T,M,M> Rtot = Rtotal<T,M> (a, tau, g, n_slab, n_slide_top, n_slide_bottom, v, w);
    for (int i = 0; i < m; i++)
        myRs += 2 * v[i] * w[i] * Rtot(i, m - 1);
    return myRs;
}

//*/

template < typename T, size_t M >
T Rborder(T n_slab, T n_slide) {
    const auto cached1 = FresnelR(n_slab, n_slide, static_cast<T>(1));
    const auto cached2 = FresnelR(n_slide, static_cast<T>(1), static_cast<T>(1));
    const auto cached3 = cached1 * cached2;
    return (cached1 + cached2 - 2 * cached3) / (1 - cached3);
}

template < typename T, size_t M >
T Tc(T tau, T n_slab, T n_slide_top, T n_slide_bottom) {
    auto Rbtop = Rborder<T,M>(n_slab, n_slide_top);
    auto Rbbottom = Rborder<T,M>(n_slab, n_slide_bottom);
    return ((1 - Rbtop) * (1 - Rbbottom) * exp(-tau)) / (1 - Rbtop * Rbbottom * exp(-2 * tau));
}
