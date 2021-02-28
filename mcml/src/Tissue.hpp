#pragma once

#include "Photon.hpp"


template<class T>
class Tissue {
public:
    T w;
    T n;
    T l;
    T att;
    T g;

    Tissue() {}
    Tissue(T _w, T _n, T _l, T _g, T _att) : w(_w), n(_n), l(_l), g(_g), att(_att) {}

    T path_length() const noexcept{ return -l*log(fast_random<T>() + 0.00001);}

    Vector3<T> scatter() const;


    T Frenel_refraction(Photon<double>& p) const;
};

template<class T>
Vector3<T> Tissue<T>::scatter() const {
    T r1 = fast_random<T>();
    T r2 = fast_random<T>();
    T cos_th;

    if(g == 0)
        cos_th = 2*r1 - 1;
    else
        cos_th = (1 + g*g - (1-g*g)/(1 - g + 2*g*r1)*(1-g*g)/(1 - g + 2*g*r1))/(2*g);

    T phi = 2*pi*r2;

    T sin_th = sqrt(1 - cos_th*cos_th);
    T x = sin_th*cos(phi);
    T y = sin_th*sin(phi);

    return Vector3<T>(x, y, cos_th);
}

template<class T>
T Tissue<T>::Frenel_refraction(Photon<double>& p) const {
    T cos_th1 = fabs(p.u.z());
    T sin_th1 = sqrt(1 - cos_th1*cos_th1);
    T sin_th2 = n*sin_th1;
    if(sin_th2 >= 1)
        return 1;
    T cos_th2 = sqrt(1 - sin_th2*sin_th2);

//    std::cerr << "\n-----------------------------------\n"
//              << "Frenel refraction code\n"
//              << cos_th1 << ' ' << sin_th1 << ' ' << cos_th2 << ' ' << sin_th2
//              << "\n-----------------------------------\n";

    return 0.5*(sin_th1*cos_th2 - cos_th1*sin_th2)*(sin_th1*cos_th2 - cos_th1*sin_th2)
            * ((cos_th1*cos_th2 + sin_th1*sin_th2)*(cos_th1*cos_th2 + sin_th1*sin_th2)
             + (cos_th1*cos_th2 - sin_th1*sin_th2)*(cos_th1*cos_th2 - sin_th1*sin_th2))
            / ((sin_th1*cos_th2 + cos_th1*sin_th2)*(sin_th1*cos_th2 + cos_th1*sin_th2)
             * (cos_th1*cos_th2 + sin_th1*sin_th2)*(cos_th1*cos_th2 + sin_th1*sin_th2));
}
