#pragma once

#include "../eigen/Eigen/Dense"
#include <iostream>

using namespace Eigen;

template <typename T>
class Photon
{
    public:
        Photon() = default;//default constructor
        Photon(const Matrix<T, 1, 3>& coord, const Matrix<T, 1, 3>& dir, const T& w, const T& path); // construct from coord, dir, weight
        ~Photon() = default; //destructor

        Matrix<T, 1, 3> coordinate = Matrix<T, 1, 3>::Constant(0.0);
        Matrix<T, 1, 3> direction = Matrix<T, 1, 3>::Constant(0.0);
        T weight = 1.0;
        T lastStep = 0;
        T pathLength = 0.0;
        int stepN = 0;
        bool alive = true;

        void printInfo ();
};

template < typename T>
Photon<T>::Photon(const Matrix<T, 1, 3>& new_coord, const Matrix<T, 1, 3>& new_dir, const T& new_w, const T& new_path)  {
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

