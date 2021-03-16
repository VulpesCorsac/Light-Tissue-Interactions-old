#ifndef ANGLES_H
#define ANGLES_H

#include <iostream>
#include <cmath>
#include <complex>

template <class T>
T Vc (T n_refr) {
    return (cos(asin (1.0/n_refr)));
}

template <class T>
T Vt (T n_i, T n_t, T Vi) {
    complex<T> comp = sqrt(complex<T>(1 - pow(n_i/n_t, 2) * (1 - pow(Vi, 2))));
    return real(comp);
}

#endif // ANGLES_H
