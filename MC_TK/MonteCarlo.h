#pragma once

#include "Photon.h"
#include "Medium.h"
#include "Fresnel.h"

#include "../Utils/Random.h"

#include "../eigen/Eigen/Dense"

#include <cmath>
#include <tgmath.h>

using namespace Eigen;

template < typename T, size_t Nz, size_t Nr >
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

    void Normalize(T& refl, T& trans);

protected:
    int Nphotons = 1e6;
    int threads = 1;

    T dz = 1e-3;
    T dr = 1e-3;
    T threshold = 1e-3;
    T chance = 0.1;

    T nVac = 1.0;

    bool debug = 1;
    int debugPhoton = 0;

    Matrix<T, Dynamic, Dynamic> A = Matrix<T, Nz, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> RR = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> TT = Matrix<T, 1, Nr>::Constant(0.0);

    Photon<T> photon; /// TODO: you use the same photon, why not passing it as a parameter to the function to be able to use multithreading later on?
    const Medium<T>& tissue;

    void Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir); //TODO: different light sources
    void FirstReflection();
    void CheckBoundaries();
    void Hop();
    void Drop();
    void Spin();
    void Terminate();
    void Simulation(const int& num);

    T Volume(const T& ir);
    T Area(const T& ir);
};

template < typename T, size_t Nz, size_t Nr >
MonteCarlo<T, Nz, Nr>::MonteCarlo(const Medium<T>& new_medium,
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

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir) {
    // std::cout << "LAUNCH" << std::endl;
    /// TODO: better set these parameters to default and use them explicitly only when needed
    photon = Photon<T>(startCoord, startDir, 1.0, 0.0);
    photon.alive = 1;
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::FirstReflection() {
    T n1 = nVac;
    T n2 = tissue.getN();
    T cos1 = std::abs(photon.direction.z);

    T Ri = FresnelR(n1, n2, cos1);
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    int ir = std::floor(r/dr);

    RR(ir) += Ri * photon.weight;
    photon.weight *= (1 - Ri);
    photon.direction.z = Cos2(n1, n2, cos1);
    photon.coordinate.z += 1e-9; //crook
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::CheckBoundaries() {
    if (photon.coordinate.z < 0) {// trying to escape -- front

        T s1 = std::abs((photon.coordinate.z) / photon.direction.z);
        T s = photon.lastStep;

        if (debug && photon.number == debugPhoton) {
            std::cout << "Reflection on front, s1 = " << s1 << std::endl;
            photon.printInfo();
        }

        photon.coordinate -= s1*photon.direction;

        T n1 = tissue.getN();
        T n2 = nVac;
        T cos1 = photon.direction.z;

        T Ri = FresnelR(n1, n2, cos1);
        T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
        int ir = std::floor(r/dr); // +1?


   /*     if (ir >= Nr)
            std::cout << "ACHTUNG!!! ir = " << ir << " exceeds Nr during boundary check" << std::endl;*/

        if (ir >= Nr)
            RR(Nr-1)+= (1 - Ri) * photon.weight; // OVERFLOW BIN
        else
            RR(ir) += (1 - Ri) * photon.weight;

        photon.weight *= Ri;

        T z = photon.direction.z;
        photon.direction.z = -z;

        photon.coordinate += (s-s1) * photon.direction;
        photon.lastStep = s1;

    } else if (photon.coordinate.z > tissue.getD()) { // trying to escape -- rear

        T s1 = std::abs((photon.coordinate.z - tissue.getD()) / photon.direction.z);
        T s = photon.lastStep;

        if (debug && photon.number == debugPhoton) {
            std::cout << "Reflection on rear, s1 = " << s1 << std::endl;
            photon.printInfo();
        }

        photon.coordinate -= s1*photon.direction;

        T n1 = tissue.getN();
        T n2 = nVac;
        T cos1 = photon.direction.z;

        T Ri = FresnelR(n1, n2, cos1);
        T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
        int ir = std::floor(r/dr); // +1?

        if (ir > Nr && debug)
            std::cout << "ACHTUNG!!! ir = " << ir << " exceeds Nr during boundary check" << std::endl;

        if (ir >= Nr)
            TT(Nr-1)+= (1 - Ri) * photon.weight; // OVERFLOW BIN
        else
            TT(ir) += (1 - Ri) * photon.weight;

        photon.weight *= Ri;

        T z = photon.direction.z;
        photon.direction.z = -z;

        photon.coordinate += (s-s1) * photon.direction;
        photon.lastStep = s1;
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Hop() {
    T RND = random(0.0, 1.0);
    T s = - std::log(RND) / tissue.getMt();
  //  T s = 0.0025;
    photon.coordinate += s * photon.direction;

    while ( photon.coordinate.z < 0 || photon.coordinate.z > tissue.getD() ) {
        if (photon.direction.z > 0) { // rear
            T s1 = (photon.coordinate.z - tissue.getD())/photon.direction.z;
            if (debug && photon.number == debugPhoton) {
                std::cout << "Reflection on rear, s1 = " << s1 << std::endl;
                photon.printInfo();
            }
            photon.coordinate -= s1 * photon.direction;
            T n1 = tissue.getN();
            T n2 = nVac;
            T cos1 = photon.direction.z;

            T Ri = FresnelR(n1, n2, cos1);
            T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));

            int ir = std::floor(r/dr);

            if (ir >= Nr)
                TT(Nr-1)+= (1 - Ri) * photon.weight; // OVERFLOW BIN
            else
                TT(ir) += (1 - Ri) * photon.weight;

            photon.weight *= Ri;

            T z = photon.direction.z;
            photon.direction.z = -z;

            photon.coordinate += s1 * photon.direction;
            photon.lastStep = s1;

       /*     if (debug && photon.number == debugPhoton) {
                std::cout << "Reflected on rear" << std::endl;
                photon.printInfo();
            }*/
        } else { // front
            T s1 = photon.coordinate.z / photon.direction.z;
            if (debug && photon.number == debugPhoton) {
                std::cout << "Reflection on front, s1 = " << s1 << std::endl;
                photon.printInfo();
            }
            photon.coordinate -= s1 * photon.direction;
            T n1 = tissue.getN();
            T n2 = nVac;
            T cos1 = photon.direction.z;

            T Ri = FresnelR(n1, n2, cos1);
            T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));

            int ir = std::floor(r/dr);

            if (ir >= Nr)
                RR(Nr-1)+= (1 - Ri) * photon.weight; // OVERFLOW BIN
            else
                RR(ir) += (1 - Ri) * photon.weight;

            photon.weight *= Ri;

            T z = photon.direction.z;
            photon.direction.z = -z;

            photon.coordinate += s1 * photon.direction;
            photon.lastStep = s1;

         /*   if (debug && photon.number == debugPhoton) {
                std::cout << "Reflected on front" << std::endl;
                photon.printInfo();
            }*/
        }
    }

    if (debug && photon.number == debugPhoton)
        std::cout << "RND = " << RND << " s = " << s << std::endl;
//    photon.coordinate += s * photon.direction;
//    photon.lastStep = s;
    photon.pathLength += s;
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Drop() {
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    int ir = std::floor(r / dr);
    int iz = std::floor(photon.coordinate.z / dz);
    if (iz < 0)
        iz = 0;

    T dw = photon.weight * tissue.getMa() / tissue.getMt();

    if (ir >= Nr && debug)
        std::cout << "ACHTUNG!!! ir = " << ir << " exceeds Nr during drop"  << std::endl;
    if (iz >= Nz )
        std::cout << "ACHTUNG!!! iz = " << iz << " exceeds Nz during drop of photon N "<< photon.number << std::endl;


    if (ir >= Nr)
        A(iz, Nr-1)+= dw; // OVERFLOW BIN
    else
        A(iz, ir) += dw;

    photon.weight -= dw;
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Spin() {
    T g = tissue.getG();
    T RND1 = random(0.0, 1.0);
    T cosHG = (1 + sqr(g) - sqr((1 - sqr(g)) / (1 - g + 2 * g * RND1))) / (2 * g);

    if (g == 0)
        cosHG = 2 * RND1 - 1;
    else if (g == 1)
        cosHG = 1.0;

    T RND2 = random(0.0, 1.0);

    T phi = 2 * M_PI * RND2; // radians

    if (debug && photon.number == debugPhoton)
        std::cout << "RND1 = " << RND1 << " cosHG = " << cosHG << " RND2 = " << RND2 << " phi = " << phi << std::endl;

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

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Terminate() {
    if (photon.weight < threshold) {
        T RND = random(0.0, 1.0);
        if (debug && photon.number == debugPhoton)
            std::cout << "Kill? RND = " << RND << std::endl;
        if (RND <= chance)
            photon.weight /= chance;
        else
            photon.alive = 0;
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Simulation(const int& num) {
    Vector3D<T> startCoord, startDir;
    startCoord = Vector3D<T>(0,0,0);
    startDir = Vector3D<T>(0,0,1);
    Launch(startCoord, startDir);
    photon.number = num;
    FirstReflection();
    while(photon.alive) {
        if (debug && photon.number == debugPhoton) {
            std::cout << "Before hop" << std::endl;
            photon.printInfo();
        }
        Hop();
        if (debug && photon.number == debugPhoton) {
            std::cout << "After hop" << std::endl;
            photon.printInfo();
        }
        Drop();
        if (debug && photon.number == debugPhoton) {
            std::cout << "After drop" << std::endl;
            photon.printInfo();
        }
        Spin();
        if (debug && photon.number == debugPhoton) {
            std::cout << "After spin" << std::endl;
            photon.printInfo();
        }
        Terminate();
    }
}

template < typename T, size_t Nz, size_t Nr >
T MonteCarlo<T, Nz, Nr>::Volume(const T& ir) {
    return Area(ir) * dz;
}

template < typename T, size_t Nz, size_t Nr >
T MonteCarlo<T, Nz, Nr>::Area(const T& ir) {
    return 2 * M_PI * (ir - 0.5) * sqr(dr);
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr >::Normalize(T& refl, T& trans) {
    for (int i = 0; i < Nphotons; i++)
        Simulation(i);

    refl = RR.sum() / Nphotons;
    trans = TT.sum() / Nphotons;

    std::cout << A.sum() / Nphotons << std::endl;

 /*   std::cout << RR.sum() / Nphotons << std::endl;
    std::cout << TT.sum() / Nphotons << std::endl;*/

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
