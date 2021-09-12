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
    return n * (std::legendre(n - 1, x) - x * std::legendre(n, x)) / (1 - sqr(x));
}
