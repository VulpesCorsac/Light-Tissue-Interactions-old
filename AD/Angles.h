#pragma once

#include "../Utils/Utils.h"

#include <cmath>
#include <complex>

template < typename T >
T Vc(T n_refr) {
    using namespace std;

    return cos(asin(1.0 / n_refr));
}

template < typename T >
T Vt(T n_i, T n_t, T Vi) {
    using namespace std;
    using namespace Utils_NS;

    return real(sqrt(complex<T>(1 - sqr(n_i / n_t) * (1 - sqr(Vi)))));
}
