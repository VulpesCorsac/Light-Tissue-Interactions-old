#pragma once

#include <cmath>

#ifndef M_PI
    #define M_PI 3.1415926535897932385
#endif

template < typename T >
inline T sqr(T x) noexcept {
    return x * x;
}

template < typename T >
T degrees_to_radians(T degrees) {
    return degrees * M_PI / 180;
}
