#pragma once

#include <math.h>

#ifndef M_PI
    #define M_PI 3.1415926535897932385
#endif

#define ALL_CONTAINER(c) begin(c), end(c)

namespace Utils_NS {
    template < typename T >
    T sqr(T x) noexcept;

    template < typename T >
    T degreesToRadians(T degrees) noexcept;

    template < typename T >
    T radiansToDegrees(T radians) noexcept;

    template < typename Container >
    int isize(const Container& c) noexcept;
}

/******************
 * IMPLEMENTATION *
 ******************/

 template < typename T >
T Utils_NS::sqr(T x) noexcept {
    return x * x;
}

template < typename T >
T Utils_NS::degreesToRadians(T degrees) noexcept {
    return degrees * M_PI / 180;
}

template < typename T >
T Utils_NS::radiansToDegrees(T radians) noexcept {
    return radians * 180 / M_PI;
}

template < typename Container >
int Utils_NS::isize(const Container& c) noexcept {
    return static_cast<int>(c.size());
}
