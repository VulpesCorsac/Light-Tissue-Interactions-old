#pragma once

#include "Vector3.h"

#include <iostream>

template < typename T >
class Photon {
public:
    /// TODO: must be deleted to avoid problems later on
    Photon() noexcept = default; // default constructor
    Photon(const Vector3D<T>& new_coord, const Vector3D<T>& new_dir, const T& w, const int& new_num) noexcept; // construct from coord, dir, weight
    ~Photon() noexcept = default; // destructor

    void printInfo ();

    Vector3D<T> coordinate = Vector3D<T>(0,0,0);
    Vector3D<T> direction = Vector3D<T>(0,0,0);
    T weight = 1.0;
    T step = 0; // current step size [m]
    T stepLeft = 0; // step size left, [-]
    int layer = 0;
    int stepN = 0;
    int number = 0;
    bool alive = true;
};

template < typename T >
Photon<T>::Photon(const Vector3D<T>& new_coord, const Vector3D<T>& new_dir, const T& new_w, const int& new_num) noexcept
    : coordinate(new_coord)
    , direction(new_dir)
    , weight(new_w)
    , step(0)
    , stepLeft(0)
    , layer(0)
    , number(new_num)
    , alive(weight > 0) {
}

template < typename T >
std::ostream& operator << (std::ostream& os, const Photon<T>& photon) noexcept {
    os << "Coordinate: "  << photon.coordinate;
    os << ", direction: " << photon.direction;
    os << ", weight = "   << photon.weight;
    os << ", number "     << photon.number;
    os << ", in layer "   << photon.layer;
    return os;
}
