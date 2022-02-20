#pragma once

#include "../Utils/Contracts.h"

#include <vector>

namespace Math_NS {
    /// \brief class for Mesh in 3D space (array of arrays of arrays)
    template < typename T >
    class Mesh3 {
    public:
        /// \param[in] N x-dimension size
        /// \param[in] M y-dimension size
        /// \param[in] L z-dimension size
        /// \param[in] defaultValue default value to set mesh cells
        Mesh3(int N = 0, int M = 0, int L = 0, T defaultValue = static_cast<T>(0)) noexcept;

        /// data value at cell data.[i][j][k]
        /// \param[in] i - x-dimension index
        /// \param[in] j - y-dimension index
        /// \param[in] k - z-dimension index
        /// \return data value at cell data.[i][j][k]
        /// \throw std::out_of_range If ASSERT_INPUT_PARAMS is defined and indices are out of corresponding ranges throws std::out_of_range
        T& value(int i, int j, int k) EXCEPT_INPUT_PARAMS;
        /// data value at cell data.[i][j][k]
        /// \param[in] i - x-dimension index
        /// \param[in] j - y-dimension index
        /// \param[in] k - z-dimension index
        /// \return data value at cell data.[i][j][k]
        /// \throw std::out_of_range If ASSERT_INPUT_PARAMS is defined and indices are out of corresponding ranges throws std::out_of_range
        const T& value(int i, int j, int k) const EXCEPT_INPUT_PARAMS;

    public:
        std::vector<std::vector<std::vector<T>>> data; ///< actual data

        int N; ///< x-dimension size
        int M; ///< y-dimension size
        int L; ///< z-dimension size
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Math_NS::Mesh3<T>::Mesh3(int N, int M, int L, T defaultValue) noexcept
    : N(N)
    , M(M)
    , L(L) {
    using namespace std;

    N = max(N, 0);
    M = max(M, 0);
    L = max(L, 0);

    data.resize(N);
    for (int i = 0; i < N; ++i) {
        data[i].resize(M);
        for (int j = 0; j < M; ++j)
            data[i][j].resize(L, defaultValue);
    }
}

template < typename T >
T& Math_NS::Mesh3<T>::value(int i, int j, int k) EXCEPT_INPUT_PARAMS {
    CHECK_RANGE_CONTRACT(0 <= i && i < N);
    CHECK_RANGE_CONTRACT(0 <= j && j < M);
    CHECK_RANGE_CONTRACT(0 <= k && k < L);

    return data[i][j][k];
}

template < typename T >
const T& Math_NS::Mesh3<T>::value(int i, int j, int k) const EXCEPT_INPUT_PARAMS {
    return const_cast<const T&>(value(i, j, k));
}
