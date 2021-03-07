#pragma once

#include "Photon.h"

#include "../Utils/Utils.h"

template < typename T >
T Frenel_refraction(Photon<T>& p, T n1, T n2, bool debug = false) noexcept {
    T cos_th1 = fabs(p.direction.e[2]);
    T sin_th1 = sqrt(1 - sqr(cos_th1));
    T sin_th2 = n1 * sin_th1 / n2;
    if (sin_th2 >= 1)
        return 1;
    T cos_th2 = sqrt(1 - sqr(sin_th2));
    if (sin_th1 == 0)
        return sqr((n2 - n1)/(n2 + n1));

    if (debug)
        std::cerr << "\n-----------------------------------\n"
                  << "Frenel refraction code\n"
                  << cos_th1 << ' ' << sin_th1 << ' ' << cos_th2 << ' ' << sin_th2
                  << "\n-----------------------------------\n";

    return 0.5*(sin_th1*cos_th2 - cos_th1*sin_th2)*(sin_th1*cos_th2 - cos_th1*sin_th2)
            * ((cos_th1*cos_th2 + sin_th1*sin_th2)*(cos_th1*cos_th2 + sin_th1*sin_th2)
             + (cos_th1*cos_th2 - sin_th1*sin_th2)*(cos_th1*cos_th2 - sin_th1*sin_th2))
            / ((sin_th1*cos_th2 + cos_th1*sin_th2)*(sin_th1*cos_th2 + cos_th1*sin_th2)
             * (cos_th1*cos_th2 + sin_th1*sin_th2)*(cos_th1*cos_th2 + sin_th1*sin_th2));
}

