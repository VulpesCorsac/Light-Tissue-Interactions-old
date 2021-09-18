#pragma once

#include <vector>
#include <string>

#ifdef ASSERT_INPUT_PARAMS
    #include <stdexcept>
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

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
        T& value(int i, int j, int k);
        /// data value at cell data.[i][j][k]
        /// \param[in] i - x-dimension index
        /// \param[in] j - y-dimension index
        /// \param[in] k - z-dimension index
        /// \return data value at cell data.[i][j][k]
        /// \throw std::out_of_range If ASSERT_INPUT_PARAMS is defined and indices are out of corresponding ranges throws std::out_of_range
        const T& value(int i, int j, int k) const;

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

    N = std::max(N, 0);
    M = std::max(M, 0);
    L = std::max(L, 0);

    data.resize(N);
    for (int i = 0; i < N; ++i) {
        data[i].resize(M);
        for (int j = 0; j < M; ++j)
            data[i][j].resize(L, defaultValue);
    }
}

template < typename T >
T& Math_NS::Mesh3<T>::value(int i, int j, int k) {
    #ifdef ASSERT_INPUT_PARAMS
        if(i < 0 || i >= N)
            throw std::out_of_range("Invalid index i: should be in range[0, " + std::to_string(N) + ") but " + std::to_string(i) + " is passed");
        if(j < 0 || j >= M)
            throw std::out_of_range("Invalid index j: should be in range[0, " + std::to_string(M) + ") but " + std::to_string(j) + " is passed");
        if(k < 0 || k >= L)
            throw std::out_of_range("Invalid index k: should be in range[0, " + std::to_string(L) + ") but " + std::to_string(k) + " is passed");
    #endif // ASSERT_INPUT_PARAMS

    return data[i][j][k];
}

template < typename T >
const T& Math_NS::Mesh3<T>::value(int i, int j, int k) const {
    #ifdef ASSERT_INPUT_PARAMS
        if(i < 0 || i >= N)
            throw std::out_of_range("Invalid index i: should be in range[0, " + std::to_string(N) + ") but " + std::to_string(i) + " is passed");
        if(j < 0 || j >= M)
            throw std::out_of_range("Invalid index j: should be in range[0, " + std::to_string(M) + ") but " + std::to_string(j) + " is passed");
        if(k < 0 || k >= L)
            throw std::out_of_range("Invalid index k: should be in range[0, " + std::to_string(L) + ") but " + std::to_string(k) + " is passed");
    #endif // ASSERT_INPUT_PARAMS

    return data[i][j][k];
}
