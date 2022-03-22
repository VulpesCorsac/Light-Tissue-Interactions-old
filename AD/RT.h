#pragma once

#include "AddingDoubling.h"
#include "LayerProperties.h"
#include "../MC/Medium.h"

#include "../Physics/Reflectance.h"
#include "../Utils/Contracts.h"
#include "../Utils/Utils.h"

#include "../eigen/Eigen/Dense"

#include <array>

#include "Eigen_NS.h"

namespace AddingDoubling_NS {
    template < typename T, size_t M >
    void RTslab(const Medium<T>& layer,
                const std::array<T,M>& v, const std::array<T,M>& w,
                Matrix<T,M,M>& Rslab, Matrix<T,M,M>& Tslab);

    template < typename T, size_t M >
    Matrix<T,M,M> Rbound(const Sample<T>& sample,
                         const std::array<T,M>& v, const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> Tbound(const Sample<T>& sample,
                         const std::array<T,M>& v, const std::array<T,M>& w);

    template < typename T, size_t M >
    void RTtotal(const Sample<T>& sample,
                 const std::array<T,M>& v, const std::array<T,M>& w,
                 Matrix<T,M,M>& Rtotal, Matrix<T,M,M>& Ttotal);

    template < typename T, size_t M >
    void RTs(const Sample<T>& sample,
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
void AddingDoubling_NS::RTslab(const Medium<T>& layer,
                               const std::array<T,M>& v, const std::array<T,M>& w,
                               Matrix<T,M,M>& Rslab, Matrix<T,M,M>& Tslab) {
    Doubling<T,M>(layer, v, w, Rslab, Tslab);
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::Rbound(const Sample<T>& sample,
                                        const std::array<T,M>& v, const std::array<T,M>& w) {
    using namespace Physics_NS;
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(sample.getNlayers() == 1 || sample.getNlayers() == 3);

    T nSlide = sample.getNslideTop();
    T nSlab = sample.getNslab();

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
Matrix<T,M,M> AddingDoubling_NS::Tbound(const Sample<T>& sample,
                                        const std::array<T,M>& v, const std::array<T,M>& w) {
    using namespace Physics_NS;
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(sample.getNlayers() == 1 || sample.getNlayers() == 3);

    const T& nSlide = sample.getNslideBottom();
    const T& nSlab = sample.getNslab();

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
void AddingDoubling_NS::RTtotal(const Sample<T>& sample,
                                const std::array<T,M>& v, const std::array<T,M>& w,
                                Matrix<T,M,M>& Rtotal, Matrix<T,M,M>& Ttotal) {
    using namespace Math_NS;

    CHECK_ARGUMENT_CONTRACT(sample.getNlayers() == 1 || sample.getNlayers() == 3);

    T nSlideTop = sample.getNslideTop();
    T nSlideBottom = sample.getNslideBottom();
    Medium<T> layer = sample.getTurbidMedium();

    const int m = M;
    Matrix<T,M,M> T02, R20, T03, R30, Rslab, Tslab, R30mod;
    RTslab<T,M>(layer, v, w, Rslab, Tslab);
    AddingBounds<T,M>(Rbound<T,M>(sample, v, w), Rslab, R20, Tbound<T,M>(sample, v, w), Tslab, T02);
    AddingBounds<T,M>(R20, Rbound<T,M>(sample, v, w), R30, T02, Tbound<T,M>(sample, v, w), Ttotal);
    R30mod = R30 - Rbound<T,M>(sample, v, w);

    for (int i = 0; i < m; i++)
        CHECK_RUNTIME_CONTRACT(sqr(DoubleAW<T,M>(v, w)(i) != 0));

    for (int i = 0; i < m; i++)
        R30mod(i, i) += Rbound<T,M>(sample, v, w)(i, i) / sqr(DoubleAW<T,M>(v, w)(i));
    Rtotal = R30mod;
}

template < typename T, size_t M >
void AddingDoubling_NS::RTs(const Sample<T>& sample,
                            const std::array<T,M>& v, const std::array<T,M>& w,
                            T& Rs, T& Ts) {

    CHECK_ARGUMENT_CONTRACT(sample.getNlayers() == 1 || sample.getNlayers() == 3);

    T nSlideTop = sample.getNslideTop();
    T nSlideBottom = sample.getNslideBottom();
    Medium<T> layer = sample.getTurbidMedium();

    const int m = M;
    Ts = Rs = 0;
    Matrix<T,M,M> Ttot, Rtot;
    RTtotal<T,M>(sample, v, w, Rtot, Ttot);
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
