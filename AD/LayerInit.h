#pragma once

#include "LayerProperties.h"
#include "RedistributionFunction.h"
#include "../MC/Medium.h"

#include "../Math/Basic.h"
#include "../Utils/Contracts.h"

#include "../eigen/Eigen/LU"

#include <array>

namespace AddingDoubling_NS {
    template < typename T, size_t M >
    Matrix<T,M,M> E();

    template < typename T, size_t M >
    Matrix<T,M,M> B(const Medium<T>& layer,
                    const std::array<T,M>& v,
                    const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> A(const Medium<T>& layer,
                    const std::array<T,M>& v,
                    const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> I(const Medium<T>& layer,
                    const std::array<T,M>& v,
                    const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> G(const Medium<T>& layer,
                    const std::array<T,M>& v,
                    const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> RR(const Medium<T>& layer,
                     const std::array<T,M>& v,
                     const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> TT(const Medium<T>& layer,
                     const std::array<T,M>& v,
                     const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,1> DoubleAW(const std::array<T,M>& v,
                           const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> RD1(const Medium<T>& layer,
                      const std::array<T,M>& v,
                      const std::array<T,M>& w);

    template < typename T, size_t M >
    Matrix<T,M,M> TD1(const Medium<T>& layer,
                      const std::array<T,M>& v,
                      const std::array<T,M>& w);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::E() {
    return Matrix<T,M,M>::Identity();
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::B(const Medium<T>& layer,
                                   const std::array<T,M>& v,
                                   const std::array<T,M>& w) {
    const int m = M;

    for (int i = 0; i < m; i++)
        CHECK_ARGUMENT_CONTRACT(v[i] != 0);

    const auto hpn = HPN<T,M>(v, layer.g);
    const auto cached = As<T,M>(layer.a, layer.g) * DTaus<T,M>(layer);

    Matrix<T,M,M> result;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            result(i, j) = cached * w[j] * hpn(i, j) / (4 * v[i]);
    return result;
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::A(const Medium<T>& layer,
                                   const std::array<T,M>& v,
                                   const std::array<T,M>& w) {
    using namespace Math_NS;

    const int m = M;

    for (int i = 0; i < m; i++)
        CHECK_ARGUMENT_CONTRACT(v[i] != 0);

    const auto hpp = HPP<T,M>(v, layer.g);
    const auto cached1 = DTaus<T,M>(layer);
    const auto cached2 = cached1 * As<T,M>(layer);

    Matrix<T,M,M> result;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            result(i, j) = KD(i, j) * cached1 / (2 * v[i]) - cached2 * w[j] * hpp(i, j) / (4 * v[i]);
    return result;
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::I(const Medium<T>& layer,
                                   const std::array<T,M>& v,
                                   const std::array<T,M>& w) {
    return (E<T,M>() + A<T,M>(layer, v, w)).inverse();
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::G(const Medium<T>& layer,
                                   const std::array<T,M>& v,
                                   const std::array<T,M>& w) {
    const auto b = B<T,M>(layer, v, w);
    return (E<T,M>() + A<T,M>(layer, v, w) - b * I<T,M>(layer, v, w) * b).inverse();
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::RR(const Medium<T>& layer,
                                    const std::array<T,M>& v,
                                    const std::array<T,M>& w) {
    return 2 * G<T,M>(layer, v, w) * B<T,M>(layer, v, w) * I<T,M>(layer, v, w);
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::TT(const Medium<T>& layer,
                                    const std::array<T,M>& v,
                                    const std::array<T,M>& w) {
    return 2 * G<T,M>(layer, v, w) - E<T,M>();
}

template < typename T, size_t M >
Matrix<T,M,1> AddingDoubling_NS::DoubleAW(const std::array<T,M>& v,
                                          const std::array<T,M>& w) {
    const int m = M;
    Matrix<T,M,1> t;
    for (int i = 0; i < m; i++)
        t(i) = 2 * v[i] * w[i];
    return t;
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::RD1(const Medium<T>& layer,
                                     const std::array<T,M>& v,
                                     const std::array<T,M>& w) {
    const int m = M;
    const auto cachedRR = RR<T,M>(layer, v, w);
    const auto cached2aw = DoubleAW<T,M>(v, w);

    for (int i = 0; i < m; i++)
        CHECK_RUNTIME_CONTRACT(cached2aw[i] != 0);

    Matrix<T,M,M> result;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            result(i, j) = cachedRR(i, j) / cached2aw(j);
    return result;
}

template < typename T, size_t M >
Matrix<T,M,M> AddingDoubling_NS::TD1(const Medium<T>& layer,
                                     const std::array<T,M>& v,
                                     const std::array<T,M>& w) {
    const int m = M;
    const auto cachedTT = TT<T,M>(layer, v, w);
    const auto cached2aw = DoubleAW<T,M>(v, w);

    for (int i = 0; i < m; i++)
        CHECK_RUNTIME_CONTRACT(cached2aw[i] != 0);

    Matrix<T,M,M> result;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            result(i, j) = cachedTT(i, j) / cached2aw(j);
    return result;
}
