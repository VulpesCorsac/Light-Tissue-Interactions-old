#pragma once

#include "../Utils/Utils.h"

// #include <complex>
#include <cmath>

template < typename T >
T CosT(T ni, T nt, T cosi) {
    using namespace std;
    using namespace Utils_NS;

    const T cached = 1 - sqr(ni / nt) * (1 - sqr(cosi));
    if (cached < 0)
        return 0;

    return cosi > 0 ? sqrt(cached) : -sqrt(cached);
    // return real(sqrt(complex<T>(1 - sqr(ni / nt) * (1 - sqr(cosi)))));
}

template < typename T >
T FresnelR(T ni, T nt, T cosi) {
    using namespace Utils_NS;

    const T cost = CosT(ni, nt, cosi);
    // std::cout << cost << std::endl;
    const T cached1 = (nt * cosi - ni * cost) / (nt * cosi + ni * cost);
    const T cached2 = (ni * cosi - nt * cost) / (ni * cosi + nt * cost);
    // std::cout << cached1 << " " << cached2 << std::endl;
    return 0.5 * (sqr(cached1) + sqr(cached2));
}

template < typename T >
T critCos(T ni, T nt) {
    using namespace std;

    return cos(asin(min(ni, nt) / max(ni, nt)));
}
