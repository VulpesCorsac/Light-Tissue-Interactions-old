#pragma once

#include "Vector3.h"

template < typename T >
class Photon {
public:
    Photon() noexcept {}
    
    Photon(Vector3<T> p, Vector3<T> d, T w) noexcept 
        : position(p)
        , direction(d)
        , weight(w) {}

    void move(T t) noexcept { 
        position += t*direction;
    }

public:
    Vector3<T> position;
    Vector3<T> direction;
    T weight;
};
