#pragma once

#include "Photon.h"
#include "Medium.h"
#include "Fresnel.h"
#include "Sandwich.h"

#include "../Utils/Random.h"

#include "../eigen/Eigen/Dense"

#include <cmath>
#include <tgmath.h>
#include <thread>
#include <mutex>

using namespace Eigen;

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
struct MCresults {
    T specularReflection, diffuseReflection, diffuseTransmission, absorbed;
    Matrix<T, Dynamic, Dynamic> matrixA ;
    Matrix<T, 1, Dynamic> arrayR;
    Matrix<T, 1, Dynamic> arrayRspecular;
    Matrix<T, 1, Dynamic> arrayT;

};

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
class MonteCarlo {
public:
    MonteCarlo() noexcept = default;
    MonteCarlo(const Sandwich<T, nLayers>& new_sample,
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

    bool debug = 1
    ;
    int debugPhoton = 0;

    Matrix<T, Dynamic, Dynamic> A = Matrix<T, Nz, Nr, nLayers>::Constant(0.0);
    Matrix<T, 1, Dynamic> RR = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> RRspecular = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> TT = Matrix<T, 1, Nr>::Constant(0.0);

    const Sandwich<T, nLayers>& sample;

    // void Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir, Photon<T>& photon); //TODO: different light sources
    void FirstReflection(Photon<T>& photon);
   // void CheckBoundaries(Photon<T>& photon);

    void HopDropSpin(Photon<T>& photon);
    void HopInGlass(Photon<T>& photon);
    void HopDropSpinInTissue(Photon<T>& photon);

    void StepSizeInGlass(Photon<T>& photon);
    void StepSizeInTissue(Photon<T>& photon);

    void RecordR(Photon<T>& photon, const T& FRefl);
    void RecordT(Photon<T>& photon, const T& FRefl);

    void Hop(Photon<T>& photon);
    void Drop(Photon<T>& photon);
    void Spin(Photon<T>& photon);

    bool HitBoundary(Photon<T>& photon);
    void CrossOrNot(Photon<T>& photon);
    void CrossUpOrNot(Photon<T>& photon);
    void CrossDownOrNot(Photon<T>& photon);

    void Roulette(Photon<T>& photon);
    void Simulation(Photon<T>& photon, const int& num);

    T Volume(const T& ir);
    T Area(const T& ir);

    MCresults<T,Nz,Nr, nLayers> results;
};

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
MonteCarlo<T, Nz, Nr, nLayers>::MonteCarlo(const Sandwich<T, nLayers>& new_sample,
                          const int& new_Np,
                          const int& new_threads,
                          const T& new_dz,
                          const T& new_dr,
                          const T& new_chance,
                          const T& new_thr)
    : sample(new_sample)
    , Nphotons(new_Np)
    , threads(new_threads)
    , dz(new_dz)
    , dr(new_dr)
    , chance(new_chance)
    , threshold(new_thr){
}

/*template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir, Photon<T>& photon) {
    // std::cout << "LAUNCH" << std::endl;
    /// TODO: better set these parameters to default and use them explicitly only when needed
    photon = Photon<T>(startCoord, startDir, 1.0, 0.0);
    photon.alive = 1;
}*/

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::FirstReflection(Photon<T>& photon) {
    T n1 = sample.getNvacUpper();
    T n2 = sample.getMedium(0).getN();
    T cos1 = std::abs(photon.direction.z);

  //  T Ri = FresnelR(n1, n2, cos1);
  T Ri = 0.040959;
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    int ir = std::floor(r/dr);

    RRspecular(ir) += Ri * photon.weight;

    photon.weight *= (1 - Ri);
    photon.direction.z = Cos2(n1, n2, cos1);
 //   photon.coordinate.z += 1e-9; //crook
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::HopDropSpin(Photon<T>& photon) {
    int layer = photon.layer;
    T mT = sample.getMedium(layer).getMt();
    if (mT == 0.0)
        HopInGlass(photon);
    else
        HopDropSpinInTissue(photon);
    Roulette(photon);
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::HopInGlass(Photon<T>& photon) {
    if (debug && photon.number == debugPhoton) {
            std::cout << "Before HopInGlass" << std::endl;
            photon.printInfo();
        }
    if (photon.direction.z == 0.0)
        photon.alive = 0;
    else{
        StepSizeInGlass(photon);
        Hop(photon);
        CrossOrNot(photon);
    }
    if (debug && photon.number == debugPhoton) {
            std::cout << "After HopInGlass" << std::endl;
            photon.printInfo();
        }
}
template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::HopDropSpinInTissue(Photon<T>& photon) {
    StepSizeInTissue(photon);
    if (HitBoundary(photon)) {
        if (debug && photon.number == debugPhoton) {
            std::cout << "Before HopInTissue" << std::endl;
            photon.printInfo();
        }
        Hop(photon);
        CrossOrNot(photon);
        if (debug && photon.number == debugPhoton) {
            std::cout << "After HopInTissue" << std::endl;
            photon.printInfo();
        }
    } else {
        if (debug && photon.number == debugPhoton) {
            std::cout << "Before Hop" << std::endl;
            photon.printInfo();
        }
        Hop(photon);
        if (debug && photon.number == debugPhoton) {
            std::cout << "After Hop" << std::endl;
            photon.printInfo();
        }
        Drop(photon);
        if (debug && photon.number == debugPhoton) {
            std::cout << "After Drop" << std::endl;
            photon.printInfo();
        }
        Spin(photon);
        if (debug && photon.number == debugPhoton) {
            std::cout << "After Spin" << std::endl;
            photon.printInfo();
        }
    }
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::StepSizeInGlass(Photon<T>& photon) {
    int layer = photon.layer;
    T uz = photon.direction.z;
    T distToBnd = 0;
    if (uz > 0.0)
        distToBnd = (sample.CurrentLowerBorderZ(layer) - photon.coordinate.z) / uz;
    else if (uz < 0.0)
        distToBnd = (sample.CurrentUpperBorderZ(layer) - photon.coordinate.z) / uz;
    else
        distToBnd = 0.0;
    photon.step = distToBnd;
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::StepSizeInTissue(Photon<T>& photon) {
    int layer = photon.layer;
    T mT = sample.getMedium(layer).getMt();
    if (photon.stepLeft == 0.0) { //new step
        T RND = random(0.0, 1.0);
        photon.step = - std::log(RND) / mT;
    } else { //leftover step
        photon.step = photon.stepLeft / mT;
    }
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::RecordR(Photon<T>& photon, const T& FRefl) {
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));

    int ir = std::floor(r/dr);

    if (ir >= Nr)
        RR(Nr-1)+= (1 - FRefl) * photon.weight; // OVERFLOW BIN
    else
        RR(ir) += (1 - FRefl) * photon.weight;
    if (debug && photon.number == debugPhoton) {
        std::cout << "RR at " << ir << " = " << RR(ir) << std::endl;
        photon.printInfo();
    }

    photon.weight *= FRefl;
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::RecordT(Photon<T>& photon, const T& FRefl) {
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));

    int ir = std::floor(r/dr);

    if (ir >= Nr)
        TT(Nr-1)+= (1 - FRefl) * photon.weight; // OVERFLOW BIN
    else
        TT(ir) += (1 - FRefl) * photon.weight;
    if (debug && photon.number == debugPhoton) {
        std::cout << "TT at " << ir << " = " << TT(ir) << std::endl;
        photon.printInfo();
    }

    photon.weight *= FRefl;
}


template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::Hop(Photon<T>& photon) {
    T s = photon.step;
    photon.coordinate += s * photon.direction;
}

/*template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::Hop(Photon<T>& photon) {
    T RND = random(0.0, 1.0);
    T s = - std::log(RND) / sample.getMedium(0).getMt();
  //  T s = 0.0025;
    photon.coordinate += s * photon.direction;

    while ( photon.coordinate.z < 0 || photon.coordinate.z > sample.getMedium(0).getD() ) {
        if (photon.direction.z > 0) { // rear
            T s1 = (photon.coordinate.z - sample.getMedium(0).getD())/photon.direction.z;
            if (debug && photon.number == debugPhoton) {
                std::cout << "Reflection on rear, s1 = " << s1 << std::endl;
                photon.printInfo();
            }
            photon.coordinate -= s1 * photon.direction;
            T n1 = sample.getMedium(0).getN();
            T n2 = sample.getNvacLower();
            T cos1 = photon.direction.z;

            T Ri = FresnelR(n1, n2, cos1);
            T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));

            int ir = std::floor(r/dr);

            if (ir >= Nr)
                TT(Nr-1)+= (1 - Ri) * photon.weight; // OVERFLOW BIN
            else
                TT(ir) += (1 - Ri) * photon.weight;
            if (debug && photon.number == debugPhoton) {
                std::cout << "TT at " << ir << " = " << TT(ir) << std::endl;
                photon.printInfo();
            }

            photon.weight *= Ri;

            T z = photon.direction.z;
            photon.direction.z = -z;

            photon.coordinate += s1 * photon.direction;

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
            T n1 = sample.getMedium(0).getN();
            T n2 = sample.getNvacUpper();
            T cos1 = photon.direction.z;

            T Ri = FresnelR(n1, n2, cos1);
            T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));

            int ir = std::floor(r/dr);

            if (ir >= Nr)
                RR(Nr-1)+= (1 - Ri) * photon.weight; // OVERFLOW BIN
            else
                RR(ir) += (1 - Ri) * photon.weight;

            if (debug && photon.number == debugPhoton) {
                std::cout << "RR at " << ir << " = " << RR(ir) << std::endl;
                photon.printInfo();
            }

            photon.weight *= Ri;

            T z = photon.direction.z;
            photon.direction.z = -z;

            photon.coordinate += s1 * photon.direction;

            if (debug && photon.number == debugPhoton) {
                std::cout << "Reflected on front" << std::endl;
                photon.printInfo();
            }
        }
    }

    if (debug && photon.number == debugPhoton)
        std::cout << "RND = " << RND << " s = " << s << std::endl;
//    photon.coordinate += s * photon.direction;
}*/

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::Drop(Photon<T>& photon) {
    int layer = photon.layer;
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    int ir = std::floor(r / dr);
    int iz = std::floor(photon.coordinate.z / dz);
    if (iz < 0)
        iz = std::abs(iz);

    if (iz >= Nz )
        std::cout << "ACHTUNG!!! iz = " << iz << " exceeds Nz during drop of photon N "<< photon.number << std::endl;

    if (ir >= Nr)
        A(iz, Nr-1)+= photon.weight * sample.getMedium(layer).getMa() / sample.getMedium(layer).getMt(); // OVERFLOW BIN
    else
        A(iz, ir) += photon.weight * sample.getMedium(layer).getMa() / sample.getMedium(layer).getMt();

    photon.weight *= sample.getMedium(layer).getMs() / sample.getMedium(layer).getMt();
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::Spin(Photon<T>& photon) {
    int layer = photon.layer;
    T g = sample.getMedium(layer).getG();
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


    if (std::abs(uz - 1.0)  < 1e-6) {
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

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
bool MonteCarlo<T, Nz, Nr, nLayers>::HitBoundary(Photon<T>& photon){
    bool hit;
    int layer = photon.layer;
    T uz = photon.direction.z;
    T mT = sample.getMedium(layer).getMt();

    T distToBnd = 0;
    if (uz > 0)
        distToBnd = (sample.CurrentLowerBorderZ(layer) - photon.coordinate.z)/uz;
    else if (uz < 0)
        distToBnd = (sample.CurrentUpperBorderZ(layer) - photon.coordinate.z)/uz;

    if (uz != 0 && photon.step > distToBnd) {
        photon.stepLeft = (photon.step - distToBnd) * mT;
        photon.step = distToBnd;
        hit = 1;
    } else
        hit = 0;
    return hit;

}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::CrossOrNot(Photon<T>& photon){
    if (photon.direction.z < 0.0)
        CrossUpOrNot(photon);
    else
        CrossDownOrNot(photon);
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::CrossUpOrNot(Photon<T>& photon){
    if (debug && photon.number == debugPhoton) {
            std::cout << "CrossUpOrNot" << std::endl;
    }
    T cos1 = photon.direction.z;
    int layer = photon.layer;
    T n1 = sample.getMedium(layer).getN();
    T n2 = 1.0;
    if (layer == 0)
        n2 = sample.getNvacUpper();
    else
        n2 = sample.getMedium(layer - 1).getN();
    T cos2 = Cos2(n1, n2, cos1);
    T Ri = FresnelR(n1, n2, cos1);
    T RND = random(0.0, 1.0); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton) {
            std::cout << "cos2 = " << cos2 << " FresnelR = "<< Ri <<" RND = "<< RND << std::endl;
        }

    if (layer == 0 && Ri < 1) { // partially transmitted -- only on sample border
        if (debug && photon.number == debugPhoton) {
            std::cout << "SAMPLE BORDER" << std::endl;
        }
        RecordR(photon, Ri);
        photon.direction.z = - photon.direction.z;
    } else if (RND > Ri) { // fully transmitted
        if (debug && photon.number == debugPhoton) {
            std::cout << "TRANSMITTED THROUGH BND" << std::endl;
        }
        photon.layer -= 1;
        photon.direction.x *= n1 / n2;
        photon.direction.y *= n1 / n2;
        photon.direction.z = cos2;
    } else { // fully reflected
        if (debug && photon.number == debugPhoton) {
            std::cout << "REFLECTED FROM BND" << std::endl;
        }
        photon.direction.z = -photon.direction.z;
    }
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::CrossDownOrNot(Photon<T>& photon){
    if (debug && photon.number == debugPhoton) {
            std::cout << "CrossDownOrNot" << std::endl;
        }
    T cos1 = photon.direction.z;
    int layer = photon.layer;
    T n1 = sample.getMedium(layer).getN();
    T n2 = 1.0;
    if (layer == nLayers - 1)
        n2 = sample.getNvacLower();
    else
        n2 = sample.getMedium(layer + 1).getN();
    T cos2 = Cos2(n1, n2, cos1);
    T Ri = FresnelR(n1, n2, cos1);
    T RND = random(0.0, 1.0); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton) {
            std::cout << "cos2 = " << cos2 << " FresnelR = "<< Ri << " RND = "<< RND << std::endl;
        }

    if (layer == (nLayers - 1) && Ri < 1) { // partially transmitted -- only on sample border
        if (debug && photon.number == debugPhoton) {
            std::cout << "SAMPLE BORDER" << std::endl;
        }
        RecordT(photon, Ri);
        photon.direction.z = - photon.direction.z;
    } else if (RND > Ri) { // fully transmitted
        if (debug && photon.number == debugPhoton) {
            std::cout << "TRANSMITTED THROUGH BND" << std::endl;
        }
        photon.layer += 1;
        photon.direction.x *= n1 / n2;
        photon.direction.y *= n1 / n2;
        photon.direction.z = cos2;
    } else {// fully reflected
        if (debug && photon.number == debugPhoton) {
            std::cout << "REFLECTED FROM BND" << std::endl;
        }
        photon.direction.z = -photon.direction.z;
    }
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::Roulette(Photon<T>& photon) {
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

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers>::Simulation(Photon<T>& photon, const int& num) {
    Vector3D<T> startCoord, startDir;
    startCoord = Vector3D<T>(0,0,0);
    startDir = Vector3D<T>(0,0,1); // normal incidence for now
    photon = Photon<T>(startCoord, startDir, 1.0, num);
    FirstReflection(photon);
    while(photon.alive) {
       HopDropSpin(photon);

       /* if (debug && photon.number == debugPhoton) {
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
        Roulette(photon);*/
    }
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
T MonteCarlo<T, Nz, Nr, nLayers>::Volume(const T& ir) {
    return Area(ir) * dz;
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
T MonteCarlo<T, Nz, Nr, nLayers>::Area(const T& ir) {
    return 2 * M_PI * (ir - 0.5) * sqr(dr);
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers >::PhotonsBunchSimulation(int Nstart, int Nfinish) {
    for (int i = Nstart; i < Nfinish; i++) {
        Photon<T> myPhoton;
        Simulation(myPhoton, i);
    }
}

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers >::Calculate() {

    for (int i = 0; i < Nphotons; i++) {
        Photon<T> myPhoton;
        Simulation(myPhoton, i);
   //    std::cout << RRspecular(0) << std::endl;
    }

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

template < typename T, size_t Nz, size_t Nr, size_t nLayers >
void MonteCarlo<T, Nz, Nr, nLayers >::printResults() {
    std::cout << "Diffuse reflection = " << results.diffuseReflection << std::endl;
    std::cout << "Specular reflection = " << results.specularReflection << std::endl;
    std::cout << "Diffuse transmission = " << results.diffuseTransmission << std::endl;
    std::cout << "Absorbed fraction = " << results.absorbed << std::endl;
}
