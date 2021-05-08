#pragma once

#include "../Utils/Utils.h"

// #include <complex>
#include <cmath>

template < typename T >
T CosT(const T& ni, const T& nt, const T& cosi) {
    using namespace std;

    const T cached = 1 - sqr(ni / nt) * (1 - sqr(cosi));
    if (cached < 0)
        return 0;
    /// TODO: ternary
    if (cosi > 0)
        return sqrt(cached);
    return -sqrt(cached);
   // return real(sqrt(complex<T>(1 - sqr(ni / nt) * (1 - sqr(cosi)))));
}

template < typename T >
T FresnelR(const T& ni, const T& nt, const T& cosi) {
    const T cost = CosT(ni, nt, cosi);
    // std::cout << cost << std::endl;
    const T cached1 = (nt * cosi - ni * cost) / (nt * cosi + ni * cost);
    const T cached2 = (ni * cosi - nt * cost) / (ni * cosi + nt * cost);
    // std::cout << cached1 << " " << cached2 << std::endl;
    return 0.5 * (sqr(cached1) + sqr(cached2));
}

template < typename T >
T critCos(const T& ni, const T& nt) {
    using namespace std;

    /// TODO: ternary
    if (ni < nt)
        return std::cos(std::asin(ni/nt));
    return std::cos(std::asin(nt/ni));
}
