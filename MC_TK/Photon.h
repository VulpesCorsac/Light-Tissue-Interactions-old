#pragma once

#include "../eigen/Eigen/Dense"
#include "Vector3.h"
#include <iostream>

using namespace Eigen;
using namespace Geo;

template <typename T>
class Photon
{
    public:
        Photon() = default;//default constructor
        Photon(const Vector3D<T>& coord, const Vector3D<T>& dir, const T& w, const T& path); // construct from coord, dir, weight
        ~Photon() = default; //destructor

        Vector3D<T> coordinate = Vector3D<T>(0,0,0);
        Vector3D<T> direction = Vector3D<T>(0,0,0);
        T weight = 1.0;
        T lastStep = 0;
        T pathLength = 0.0;
        int stepN = 0;
        int number = 0;
        bool alive = true;

        void printInfo ();
};

template < typename T>
Photon<T>::Photon(const Vector3D<T>& new_coord, const Vector3D<T>& new_dir, const T& new_w, const T& new_path)  {
    coordinate = new_coord;
    direction = new_dir;
    weight = new_w;
    pathLength = new_path;
    alive = (weight > 0);
    lastStep = 0;
}

template < typename T>
void Photon<T>::printInfo() {
    std::cout << "Coordinate: " << coordinate << ", direction: " << direction << ", weight = " << weight << ", last step = " << lastStep << ", path length = " << pathLength << std::endl;
}

