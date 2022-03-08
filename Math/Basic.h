#pragma once

#include <math.h>

#ifndef M_PI
    #define M_PI 3.1415926535897932385
#endif

namespace Math_NS {
    template < typename T >
    T sqr(T x) noexcept;

    template < typename T >
    T degreesToRadians(T degrees) noexcept;

    template < typename T >
    T radiansToDegrees(T radians) noexcept;

    template < typename T >
    T legendreDerivative(int n, T x);

    template < typename T >
    T KD(T i, T j) noexcept;
}

/******************
 * IMPLEMENTATION *
 ******************/

 template < typename T >
T Math_NS::sqr(T x) noexcept {
    return x * x;
}

template < typename T >
T Math_NS::degreesToRadians(T degrees) noexcept {
    return degrees * M_PI / 180;
}

template < typename T >
T Math_NS::radiansToDegrees(T radians) noexcept {
    return radians * 180 / M_PI;
}

template < typename T >
T Math_NS::legendreDerivative(int n, T x) {
    using namespace std;

    /// TODO: why this assertion fails on tests?
    // assert(sqr(x) != 1);
    return n * (legendre(n-1, x) - x * legendre(n, x)) / (1 - sqr(x));
}

template < typename T >
T Math_NS::KD(T i, T j) noexcept {
    return i == j ? 1 : 0;
}
