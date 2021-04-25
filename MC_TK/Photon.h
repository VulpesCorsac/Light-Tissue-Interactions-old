#pragma once

#include "Vector3.h"

#include <iostream>

template < typename T >
class Photon {
public:
    Photon() noexcept = default; //default constructor
    Photon(const Vector3D<T>& coord, const Vector3D<T>& dir, const T& w, const T& path) noexcept; // construct from coord, dir, weight
    ~Photon() noexcept = default; //destructor

    void printInfo ();

public:
    Vector3D<T> coordinate = Vector3D<T>(0,0,0);
    Vector3D<T> direction = Vector3D<T>(0,0,0);
    T weight = 1.0;
    T lastStep = 0;
    T pathLength = 0.0;
    int stepN = 0;
    int number = 0;
    bool alive = true;
};

template < typename T >
Photon<T>::Photon(const Vector3D<T>& new_coord, const Vector3D<T>& new_dir, const T& new_w, const T& new_path) noexcept {
    coordinate = new_coord;
    direction = new_dir;
    weight = new_w;
    pathLength = new_path;
    alive = (weight > 0);
    lastStep = 0;
}

template < typename T >
void Photon<T>::printInfo() {
    std::cout << "Coordinate: " << coordinate << ", direction: " << direction << ", weight = " << weight << ", last step = " << lastStep << ", path length = " << pathLength << std::endl;
}
