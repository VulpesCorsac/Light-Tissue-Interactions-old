#pragma once

#include "AddingDoubling.h"

#include "../Physics/Reflectance.h"
#include "../Utils/Contracts.h"
#include "../Utils/Utils.h"

#include "../eigen/Eigen/Dense"

#include <array>

#include "Eigen_NS.h"

namespace AddingDoubling_NS {
    template < typename T, size_t M >
    void RTslab(T a, T tau, T g, T nSlab,
                const std::array<T,M>& v, const std::array<T,M>& w,
                Matrix<T,M,M>& Rslab, Matrix<T,M,M>& Tslab);

    template < typename T, size_t M >
    Matrix<T,M,M> Rbound(T a, T tau, T g, T nSlab, T nSlide,
                         const std::array<T,M>& v, const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> Tbound(T a, T tau, T g, T nSlab, T nSlide,
                         const std::array<T,M>& v, const std::array<T,M>& w);

    template < typename T, size_t M >
    void RTtotal(T a, T tau, T g, T nSlab, T nSlideTop, T nSlideBottom,
                 const std::array<T,M>& v, const std::array<T,M>& w,
                 Matrix<T,M,M>& Rtotal, Matrix<T,M,M>& Ttotal);

    template < typename T, size_t M >
    void RTs(T a, T tau, T g, T nSlab, T nSlideTop, T nSlideBottom,
             const std::array<T,M>& v, const std::array<T,M>& w,
             T& Rs, T& Ts);

    template < typename T, size_t M >
    T Rborder(T nSlab, T nSlide);

    template < typename T, size_t M >
    T Tc(T tau, T nSlab, T nSlideTop, T nSlideBottom);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T, size_t M >
void AddingDoubling_NS::RTslab(T a, T tau, T g,
                               T nSlab,
                               const std::array<T,M>& v, const std::array<T,M>& w,
                               Matrix<T,M,M>& Rslab, Matrix<T,M,M>& Tslab) {
    Doubling<T,M>(a, tau, g, nSlab, v, w, Rslab, Tslab);
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::Rbound(T a, T tau, T g,
                                        T nSlab, T nSlide,
                                        const std::array<T,M>& v, const std::array<T,M>& w) {
    using namespace Physics_NS;
    using namespace std;

    /// TODO: a, tau, g - unused!
    ignore = a;
    ignore = tau;
    ignore = g;

    const int m = M;
    Matrix<T,M,M> result = E<T,M>();
    for (int i = 0; i < m; i++) {
        const auto cached1 = FresnelReflectance(nSlide, static_cast<T>(1), TransmittanceCos(nSlab, nSlide, v[i]));
        const auto cached2 = FresnelReflectance(nSlab , nSlide           , v[i]                                 );
        const auto cached3 = cached1 * cached2;

        CHECK_RUNTIME_CONTRACT(cached3 != 1);

        result(i, i) = real(DoubleAW<T,M>(v, w)(i) * (cached2 + cached1 - 2 * cached3)) / (1 - cached3);
    }
    return result;
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::Tbound(T a, T tau, T g,
                                        T nSlab, T nSlide,
                                        const std::array<T,M>& v, const std::array<T,M>& w) {
    using namespace Physics_NS;
    using namespace std;

    /// TODO: a, tau, g, w - unused!
    ignore = a;
    ignore = tau;
    ignore = g;
    ignore = w;

    const int m = M;
    Matrix<T,M,M> result = E<T,M>();
    for (int i = 0; i < m; i++) {
        const auto cached1 = FresnelReflectance(nSlide, static_cast<T>(1), TransmittanceCos(nSlab, nSlide, v[i]));
        const auto cached2 = FresnelReflectance(nSlab , nSlide           , v[i]                                 );
        const auto cached3 = cached1 * cached2;

        CHECK_RUNTIME_CONTRACT(cached3 != 1);

        result(i, i) = real(1 - (cached2 + cached1 - 2 * cached3) / (1 - cached3));
    }
    return result;
}

template < typename T, size_t M >
void AddingDoubling_NS::RTtotal(T a, T tau, T g,
                                T nSlab, T nSlideTop, T nSlideBottom,
                                const std::array<T,M>& v, const std::array<T,M>& w,
                                Matrix<T,M,M>& Rtotal, Matrix<T,M,M>& Ttotal) {
    using namespace Math_NS;

    const int m = M;
    Matrix<T,M,M> T02, R20, T03, R30, Rslab, Tslab, R30mod;
    RTslab<T,M>(a, tau, g, nSlab, v, w, Rslab, Tslab);
    AddingBounds<T,M>(Rbound<T,M>(a, tau, g, nSlab, nSlideTop, v, w), Rslab, R20, Tbound<T,M>(a, tau, g, nSlab, nSlideTop, v, w), Tslab, T02);
    AddingBounds<T,M>(R20, Rbound<T,M>(a, tau, g, nSlab, nSlideBottom, v, w), R30, T02, Tbound<T,M>(a, tau, g, nSlab, nSlideBottom, v, w), Ttotal);
    R30mod = R30 - Rbound<T,M>(a, tau, g, nSlab, nSlideBottom, v, w);

    for (int i = 0; i < m; i++)
        CHECK_RUNTIME_CONTRACT(sqr(DoubleAW<T,M>(v, w)(i) != 0));

    for (int i = 0; i < m; i++)
        R30mod(i, i) += Rbound<T,M>(a, tau, g, nSlab, nSlideBottom, v, w)(i, i) / sqr(DoubleAW<T,M>(v, w)(i));
    Rtotal = R30mod;
}

template < typename T, size_t M >
void AddingDoubling_NS::RTs(T a, T tau, T g,
                            T nSlab, T nSlideTop, T nSlideBottom,
                            const std::array<T,M>& v, const std::array<T,M>& w,
                            T& Rs, T& Ts) {
    const int m = M;
    Ts = Rs = 0;
    Matrix<T,M,M> Ttot, Rtot;
    RTtotal<T,M>(a, tau, g, nSlab, nSlideTop, nSlideBottom, v, w, Rtot, Ttot);
    for (int i = 0; i < m; i++) {
        Ts += 2 * v[i] * w[i] * Ttot(i, m-1);
        Rs += 2 * v[i] * w[i] * Rtot(i, m-1);
    }
}

template < typename T, size_t M >
T AddingDoubling_NS::Rborder(T nSlab, T nSlide) {
    using namespace Physics_NS;

    const auto cached1 = FresnelReflectance(nSlab , nSlide           , static_cast<T>(1));
    const auto cached2 = FresnelReflectance(nSlide, static_cast<T>(1), static_cast<T>(1));
    const auto cached3 = cached1 * cached2;

    CHECK_RUNTIME_CONTRACT(cached3 != 1);

    return (cached1 + cached2 - 2 * cached3) / (1 - cached3);
}

template < typename T, size_t M >
T AddingDoubling_NS::Tc(T tau, T nSlab, T nSlideTop, T nSlideBottom) {
    const auto Rbtop    = Rborder<T,M>(nSlab, nSlideTop   );
    const auto Rbbottom = Rborder<T,M>(nSlab, nSlideBottom);

    CHECK_RUNTIME_CONTRACT(Rbtop * Rbbottom * exp(-2 * tau) != 1);

    return ((1 - Rbtop) * (1 - Rbbottom) * exp(-tau)) / (1 - Rbtop * Rbbottom * exp(-2 * tau));
}
