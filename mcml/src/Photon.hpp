#pragma once

#include "Vector3.hpp"
#include "General.hpp"

#include <cmath>


template<class T>
class Photon {
public:
    Vector3<T> position;
    Vector3<T> direction;
    T weight;

    Photon(){}
    Photon(Vector3<T> _p, Vector3<T> _d, T _w) : position(_p), direction(_d), weight(_w) {}

    void move(T t) noexcept { position += t*direction;}
};

