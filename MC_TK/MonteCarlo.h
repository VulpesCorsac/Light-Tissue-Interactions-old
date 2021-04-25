#pragma once

#include "Photon.h"
#include "Medium.h"
#include "Fresnel.h"

#include "../Utils/Random.h"

#include "../eigen/Eigen/Dense"

#include <cmath>
#include <tgmath.h>

using namespace Eigen;

template < typename T >
class MonteCarlo {
public:
    MonteCarlo() noexcept = default;
    MonteCarlo(const Medium<T>& new_medium,
               const int& new_Np,
               const int& new_threads,
               const T& new_dz,
               const T& new_dr,
               const T& new_chance,
               const T& new_thr);
    ~MonteCarlo() noexcept = default;

    void Normalize();

protected:
    int Nphotons = 1e6;
    int threads = 1;

    T dz = 1e-3;
    T dr = 1e-3;
    T threshold = 1e-3;
    T chance = 0.1;

    T nVac = 1.0;


    Matrix<T, Dynamic, Dynamic> A = Matrix<T, 500, 2000>::Constant(0.0);
    Matrix<T, 1, Dynamic> RR = Matrix<T, 1, 2000>::Constant(0.0);
    Matrix<T, 1, Dynamic> TT = Matrix<T, 1, 2000>::Constant(0.0);

    Photon<T> photon; /// TODO: you use the same photon, why not passing it as a parameter to the function to be able to use multithreading later on?
    const Medium<T>& tissue;

    void Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir); //TODO: different light sources
    void Hop();
    void CheckBoundaries();
    void Drop();
    void Spin();
    void Terminate();
    void Simulation(const int& debug);

    T Volume(const T& ir);
    T Area(const T& ir);
};

template < typename T >
MonteCarlo<T>::MonteCarlo(const Medium<T>& new_medium,
                          const int& new_Np,
                          const int& new_threads,
                          const T& new_dz,
                          const T& new_dr,
                          const T& new_chance,
                          const T& new_thr)
    : tissue(new_medium)
    , Nphotons(new_Np)
    , threads(new_threads)
    , dz(new_dz)
    , dr(new_dr)
    , chance(new_chance)
    , threshold(new_thr)
    , nVac(1.0) {
}

template < typename T >
void MonteCarlo<T>::Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir) {
    // std::cout << "LAUNCH" << std::endl;
    /// TODO: better set these parameters to default and use them explicitly only when needed
    photon = Photon<T>(startCoord, startDir, 1.0, 0.0);
    photon.alive = 1;
}

template < typename T >
void MonteCarlo<T>::Hop() {
    T RND = random(0.0, 1.0);
    T s = - std::log(RND) / tissue.getMt();
    photon.coordinate += s * photon.direction;
    photon.lastStep = s;
    photon.pathLength += s;
}

template < typename T >
void MonteCarlo<T>::CheckBoundaries() {
    if (photon.coordinate.z < 0) { // trying to escape -- front
        T s1 = std::abs(photon.coordinate.z / photon.direction.z);
        /*
        photon.coordinate -= photon.lastStep * photon.direction;
        photon.pathLength -= photon.lastStep; // maybe i'll get rid of it
        photon.coordinate += s1 * photon.direction;
        //*/
        photon.coordinate -= s1*photon.direction;

        T n1 = tissue.getN();
        T n2 = nVac;
        T cos1 = std::abs(photon.direction.z);

        T Ri = FresnelR(n1, n2, cos1);
        T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
        int ir = std::floor(r/dr); // +1?

        /*
        if (ir > Nr)
            std::cout << "ACHTUNG!!! ir exceeds Nr during boundary check" << std::endl;
        //*/

        RR(ir) += (1 - Ri) * photon.weight;
        photon.weight *= Ri;
        photon.direction.z = -photon.direction.z;

        photon.coordinate = (photon.lastStep - s1) * photon.direction;
        photon.lastStep -= s1;
    } else if (photon.coordinate.z > tissue.getD()) { // trying to escape -- rear
        T s1 = std::abs((photon.coordinate.z-tissue.getD()) / photon.direction.z);
        /*
        photon.coordinate -= photon.lastStep * photon.direction;
        photon.coordinate += s1 * photon.direction;
        //*/
        photon.coordinate -= s1*photon.direction;

        T n1 = tissue.getN();
        T n2 = nVac;
        T cos1 = std::abs(photon.direction.z);

        T Ri = FresnelR(n1, n2, cos1);
        T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
        int ir = std::floor(r/dr); // +1?

        /*
        if (ir > Nr)
            std::cout << "ACHTUNG!!! ir exceeds Nr during boundary check" << std::endl;
        //*/

        TT(ir) += (1 - Ri) * photon.weight;
        photon.weight *= Ri;
        photon.direction.z = -photon.direction.z;

        photon.coordinate += (photon.lastStep - s1) * photon.direction;
        photon.lastStep -= s1;
    }
}

template < typename T >
void MonteCarlo<T>::Drop() {
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    int ir = std::floor(r / dr);
    int iz = std::floor(photon.coordinate.z / dz);
    if (iz < 0)
        iz = 0;

    /*
    if (ir >= Nr)
        std::cout << "ACHTUNG!!! ir exceeds Nr during drop" << std::endl;
    if (iz >= Nz)
        std::cout << "ACHTUNG!!! iz exceeds Nz during drop" << std::endl;
    //*/

    A(iz, ir) += photon.weight * tissue.getMa() / tissue.getMt();
    photon.weight *= tissue.getMs() / tissue.getMt();
}

template < typename T >
void MonteCarlo<T>::Spin() {
    T g = tissue.getG();
    /// TODO: You actually calculate random once, but use at least twice!
    T RND = random(0.0, 1.0);
    T cosHG = (1 + sqr(g) - sqr((1 - sqr(g)) / (1 - g + 2 * g * RND))) / (2 * g);

    if (g == 0)
        cosHG = 2 * RND - 1;
    else if (g == 1)
        cosHG = 1.0;

    T phi = 2 * M_PI * RND; // radians

    T ux = photon.direction.x;
    T uy = photon.direction.y;
    T uz = photon.direction.z;
    T sinHG = std::sqrt(1 - sqr(cosHG));
    T temp = std::sqrt(1 - sqr(uz));

    T uxx = +sinHG * (ux*uz*std::cos(phi) - uy*std::sin(phi)) / temp + ux * cosHG;
    T uyy = +sinHG * (uy*uz*std::cos(phi) + ux*std::sin(phi)) / temp + uy * cosHG;
    T uzz = -sinHG * std::cos(phi) * temp                            + uz * cosHG;

    if (std::abs(uz) - 1.0 < 1e-3) {
        uxx = sinHG * std::cos(phi);
        uyy = sinHG * std::sin(phi);
        if (uz >= 0)
            uzz = cosHG;
        else
            uzz = -cosHG;
    }

    photon.direction.x = uxx;
    photon.direction.y = uyy;
    photon.direction.z = uzz;
}

template < typename T >
void MonteCarlo<T>::Terminate() {
    if (photon.weight < threshold) {
        T RND = random(0.0, 1.0);
        if (RND <= chance)
            photon.weight /= chance;
        else
            photon.alive = 0;
    }
}

template < typename T >
void MonteCarlo<T>::Simulation(const int& debug) {
    Vector3D<T> startCoord, startDir;
    startCoord = Vector3D<T>(0,0,0);
    startDir = Vector3D<T>(0,0,1);
    Launch(startCoord, startDir);
    photon.number = debug;
    while(photon.alive) {
        Hop();
        CheckBoundaries();
        Drop();
        Spin();
        Terminate();
    }
}

template < typename T >
T MonteCarlo<T>::Volume(const T& ir) {
    return Area(ir) * dz;
}

template < typename T >
T MonteCarlo<T>::Area(const T& ir) {
    return 2 * M_PI * (ir - 0.5) * sqr(dr);
}

template < typename T >
void MonteCarlo<T>::Normalize() {
    for (int i = 0; i < Nphotons; i++)
        Simulation(i);

    /*
    std::cout << "R matrix" << std::endl;
    std::cout << RR << std::endl;
    std::cout << "T matrix" << std::endl;
    std::cout << TT << std::endl;
    std::cout << "A matrix" << std::endl;
    std::cout << A << std::endl;
    //*/

    std::cout << RR.sum() / Nphotons << std::endl;
    std::cout << TT.sum() / Nphotons << std::endl;

    /*
    for (int i = 0; i < Nz; i++)
        for (int j = 0; j < Nr; j++)
            A(i,j) /= Volume(j+1) * Nphotons;

    for(int j = 0; j < Nr; j++) {
        RR(j) /= Area(j+1) * Nphotons;
        TT(j) /= Area(j+1) * Nphotons;
    }
    //*/
}
