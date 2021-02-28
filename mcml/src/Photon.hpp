#pragma once

#include <cmath>

#include "Vector3.hpp"
#include "General.hpp"


template<class T>
class Photon {
public:
    Vector3<T> p;
    Vector3<T> u;
    T w;

    Photon(){}
    Photon(Vector3<T> _p, Vector3<T> _u, T _w) : p(_p), u(_u), w(_w) {}

    void move(T t) noexcept { p += t*u;}
};

