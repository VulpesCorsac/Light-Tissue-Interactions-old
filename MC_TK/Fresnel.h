#pragma once

#include "../Utils/Utils.h"

#include <complex>
#include <cmath>

template < typename T >
T FresnelR(const T& n1, const T& n2, const T& cos1) {
    T cos2 = real(std::sqrt(std::complex<T>(1 - sqr(n1 / n2) * (1 - sqr(cos1)))));
    T cached1 = (n2*cos1 - n1*cos2) / (n2*cos1 + n1*cos2);
    T cached2 = (n1*cos1 - n2*cos2) / (n1*cos1 + n2*cos2);
    return 0.5*sqr(cached1) + 0.5*sqr(cached2);
}
