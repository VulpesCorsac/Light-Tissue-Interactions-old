#pragma once

#include "Vector3.h"

#include <iostream>

template < typename T >
class Photon {
public:
    Photon() noexcept = default; //default constructor
    Photon(const Vector3D<T>& coord, const Vector3D<T>& dir, const T& w, const int& num) noexcept; // construct from coord, dir, weight
    ~Photon() noexcept = default; //destructor

    void printInfo ();

public:
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
Photon<T>::Photon(const Vector3D<T>& new_coord, const Vector3D<T>& new_dir, const T& new_w, const int& new_num) noexcept {
    coordinate = new_coord;
    direction = new_dir;
    weight = new_w;
    number = new_num;
    alive = (weight > 0);
    step = 0;
    stepLeft = 0;
    layer = 0;
}

template < typename T >
void Photon<T>::printInfo() {
    std::cout << "Coordinate: " << coordinate << ", direction: " << direction << ", weight = " << weight  << ", number " << number << ", in layer " << layer << std::endl;
}
