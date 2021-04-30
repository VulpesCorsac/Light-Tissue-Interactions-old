#pragma once

#include "Photon.h"
#include "Medium.h"
#include "Fresnel.h"

#include "../Utils/Random.h"

#include "../eigen/Eigen/Dense"

#include <cmath>
#include <tgmath.h>
#include <thread>
#include <mutex>

using namespace Eigen;

template < typename T, size_t Nz, size_t Nr >
struct MCresults {
    T specularReflection, diffuseReflection, diffuseTransmission, absorbed;
    Matrix<T, Dynamic, Dynamic> matrixA ;
    Matrix<T, 1, Dynamic> arrayR;
    Matrix<T, 1, Dynamic> arrayRspecular;
    Matrix<T, 1, Dynamic> arrayT;

};

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

    void PhotonsBunchSimulation(int Nstart, int Nfinish);
    void Calculate();
    void printResults();

protected:
    int Nphotons = 1e6;
    int threads = 1;

    T dz = 1e-3;
    T dr = 1e-3;
    T threshold = 1e-3;
    T chance = 0.1;

    T nVac = 1.0;

    bool debug = 0;
    int debugPhoton = 50000;

    Matrix<T, Dynamic, Dynamic> A = Matrix<T, Nz, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> RR = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> RRspecular = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> TT = Matrix<T, 1, Nr>::Constant(0.0);

    const Medium<T>& tissue;

    void Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir, Photon<T>& photon); //TODO: different light sources
    void FirstReflection(Photon<T>& photon);
   // void CheckBoundaries(Photon<T>& photon);
    void Hop(Photon<T>& photon);
    void Drop(Photon<T>& photon);
    void Spin(Photon<T>& photon);
    void Terminate(Photon<T>& photon);
    void Simulation(Photon<T>& photon, const int& num);

    T Volume(const T& ir);
    T Area(const T& ir);

    MCresults<T,Nz,Nr> results;
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

/*template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir, Photon<T>& photon) {
    // std::cout << "LAUNCH" << std::endl;
    /// TODO: better set these parameters to default and use them explicitly only when needed
    photon = Photon<T>(startCoord, startDir, 1.0, 0.0);
    photon.alive = 1;
}*/

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::FirstReflection(Photon<T>& photon) {
    T n1 = nVac;
    T n2 = tissue.getN();
    T cos1 = std::abs(photon.direction.z);

    T Ri = FresnelR(n1, n2, cos1);
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    int ir = std::floor(r/dr);

    static std::mutex mtx;
    mtx.lock();
    RRspecular(ir) += Ri * photon.weight;
    mtx.unlock();

    photon.weight *= (1 - Ri);
    photon.direction.z = Cos2(n1, n2, cos1);
    photon.coordinate.z += 1e-9; //crook
}


template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Hop(Photon<T>& photon) {
    T RND = random(0.0, 1.0);
 //T RND = fast_random<T>();
    T s = - std::log(RND) / tissue.getMt();
  //  T s = 0.0025;
    photon.coordinate += s * photon.direction;
    photon.lastStep = s;

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

            static std::mutex mtx;
            mtx.lock();
            if (ir >= Nr)
                TT(Nr-1)+= (1 - Ri) * photon.weight; // OVERFLOW BIN
            else
                TT(ir) += (1 - Ri) * photon.weight;
            if (debug && photon.number == debugPhoton) {
                std::cout << "TT at " << ir << " = " << TT(ir) << std::endl;
                photon.printInfo();
            }
            mtx.unlock();

            photon.weight *= Ri;

            T z = photon.direction.z;
            photon.direction.z = -z;

            photon.coordinate += s1 * photon.direction;
            photon.lastStep = s1;

            if (debug && photon.number == debugPhoton) {
                std::cout << "Reflected on rear" << std::endl;
                photon.printInfo();
            }
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

            static std::mutex mtx;
            mtx.lock();
            if (ir >= Nr)
                RR(Nr-1)+= (1 - Ri) * photon.weight; // OVERFLOW BIN
            else
                RR(ir) += (1 - Ri) * photon.weight;
            mtx.unlock();

            if (debug && photon.number == debugPhoton) {
                std::cout << "RR at " << ir << " = " << RR(ir) << std::endl;
                photon.printInfo();
            }

            photon.weight *= Ri;

            T z = photon.direction.z;
            photon.direction.z = -z;

            photon.coordinate += s1 * photon.direction;
            photon.lastStep = s1;

            if (debug && photon.number == debugPhoton) {
                std::cout << "Reflected on front" << std::endl;
                photon.printInfo();
            }
        }
    }

    if (debug && photon.number == debugPhoton)
        std::cout << "RND = " << RND << " s = " << s << std::endl;
//    photon.coordinate += s * photon.direction;
//    photon.lastStep = s;
    photon.pathLength += s;
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Drop(Photon<T>& photon) {
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    int ir = std::floor(r / dr);
    int iz = std::floor(photon.coordinate.z / dz);
    if (iz < 0)
        iz = std::abs(iz);


/*    if (ir >= Nr && debug)
        std::cout << "ACHTUNG!!! ir = " << ir << " exceeds Nr during drop"  << std::endl;
  */  if (iz >= Nz )
        std::cout << "ACHTUNG!!! iz = " << iz << " exceeds Nz during drop of photon N "<< photon.number << std::endl;

    static std::mutex mtx;
    mtx.lock();
    if (ir >= Nr)
        A(iz, Nr-1)+= photon.weight * tissue.getMa() / tissue.getMt(); // OVERFLOW BIN
    else
        A(iz, ir) += photon.weight * tissue.getMa() / tissue.getMt();
    mtx.unlock();

    photon.weight *= tissue.getMs() / tissue.getMt();
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Spin(Photon<T>& photon) {
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

    if (std::abs(uz) - 1.0 < 1e-6) {
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
void MonteCarlo<T, Nz, Nr>::Terminate(Photon<T>& photon) {
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
void MonteCarlo<T, Nz, Nr>::Simulation(Photon<T>& photon, const int& num) {
    Vector3D<T> startCoord, startDir;
    startCoord = Vector3D<T>(0,0,0);
    startDir = Vector3D<T>(0,0,1); // normal incidence for now
    photon = Photon<T>(startCoord, startDir, 1.0, 0.0, num);
    FirstReflection(photon);
    while(photon.alive) {
        if (debug && photon.number == debugPhoton) {
            std::cout << "Before hop" << std::endl;
            photon.printInfo();
        }
        Hop(photon);
        if (debug && photon.number == debugPhoton) {
            std::cout << "After hop" << std::endl;
            photon.printInfo();
        }
        Drop(photon);
        if (debug && photon.number == debugPhoton) {
            std::cout << "After drop" << std::endl;
            photon.printInfo();
        }
        Spin(photon);
        if (debug && photon.number == debugPhoton) {
            std::cout << "After spin" << std::endl;
            photon.printInfo();
        }
        Terminate(photon);
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
void MonteCarlo<T, Nz, Nr >::PhotonsBunchSimulation(int Nstart, int Nfinish) {
    for (int i = Nstart; i < Nfinish; i++) {
        Photon<T> myPhoton;
        Simulation(myPhoton, i);
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr >::Calculate() {

    results.arrayR = RR;
    results.arrayRspecular = RRspecular;
    results.arrayT = TT;
    results.matrixA = A;

    results.diffuseReflection = RR.sum() / Nphotons;
    results.specularReflection = RRspecular.sum() / Nphotons;
    results.diffuseTransmission = TT.sum() / Nphotons;
    results.absorbed = A.sum() / Nphotons;

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

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr >::printResults() {
    std::cout << "Diffuse reflection = " << results.diffuseReflection << std::endl;
    std::cout << "Specular reflection = " << results.specularReflection << std::endl;
    std::cout << "Diffuse transmission = " << results.diffuseTransmission << std::endl;
    std::cout << "Absorbed fraction = " << results.absorbed << std::endl;
}
