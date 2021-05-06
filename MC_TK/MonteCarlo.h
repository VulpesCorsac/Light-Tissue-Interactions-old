#pragma once

#include "Photon.h"
#include "Medium.h"
#include "Fresnel.h"
#include "Sample.h"

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
    MonteCarlo(const Sample<T>& new_sample,
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

    bool debug = 0;
    int debugPhoton = 0;

    Matrix<T, Dynamic, Dynamic> A = Matrix<T, Nz, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> RR = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> RRspecular = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> TT = Matrix<T, 1, Nr>::Constant(0.0);

    const Sample<T>& sample;

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

    MCresults<T,Nz,Nr> results;
};

template < typename T, size_t Nz, size_t Nr >
MonteCarlo<T, Nz, Nr>::MonteCarlo(const Sample<T>& new_sample,
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

/*template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir, Photon<T>& photon) {
    // std::cout << "LAUNCH" << std::endl;
    /// TODO: better set these parameters to default and use them explicitly only when needed
    photon = Photon<T>(startCoord, startDir, 1.0, 0.0);
    photon.alive = 1;
}*/

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::FirstReflection(Photon<T>& photon) {
    T ni = sample.getNvacUpper();
    T nt = sample.getMedium(0).n;
    T cosi = std::abs(photon.direction.z);
    T cost = CosT(ni, nt, cosi);

    T Ri = FresnelR(ni, nt, cosi);

    if (sample.getMedium(0).ut == 0) { // specular from glass
        T R1 = FresnelR(ni, nt, cosi);
        T n3 = sample.getMedium(1).n;
        T R2 = FresnelR(nt, n3, cost);
        Ri = R1 + (sqr(1 - R1) * R2) / (1 - R1 * R2);
    }


    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    int ir = std::floor(r/dr);

    RRspecular(ir) += Ri * photon.weight;

    photon.weight *= (1 - Ri);
    photon.direction.z = CosT(ni, nt, cosi);
 //   photon.coordinate.z += 1e-9; //crook
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::HopDropSpin(Photon<T>& photon) {
    int layer = photon.layer;
    T mT = sample.getMedium(layer).ut;
    if (mT == 0.0)
        HopInGlass(photon);
    else
        HopDropSpinInTissue(photon);
    Roulette(photon);
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::HopInGlass(Photon<T>& photon) {
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
template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::HopDropSpinInTissue(Photon<T>& photon) {
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

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::StepSizeInGlass(Photon<T>& photon) {
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

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::StepSizeInTissue(Photon<T>& photon) {
    int layer = photon.layer;
    T mT = sample.getMedium(layer).ut;
    if (photon.stepLeft == 0.0) { //new step
        T RND = random(0.0, 1.0);
        photon.step = - std::log(RND) / mT;
    } else { //leftover step
        photon.step = photon.stepLeft / mT;
        photon.stepLeft = 0.0;
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::RecordR(Photon<T>& photon, const T& FRefl) {
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

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::RecordT(Photon<T>& photon, const T& FRefl) {
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


template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Hop(Photon<T>& photon) {
    T s = photon.step;
    photon.coordinate += s * photon.direction;
}


template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Drop(Photon<T>& photon) {
    int layer = photon.layer;
    T r = std::sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    int ir = std::floor(r / dr);
    int iz = std::floor(photon.coordinate.z / dz);
    if (iz < 0)
        iz = std::abs(iz);

    if (iz >= Nz )
        std::cout << "ACHTUNG!!! iz = " << iz << " exceeds Nz during drop of photon N "<< photon.number << std::endl;

    if (ir >= Nr)
        A(iz, Nr-1)+= photon.weight * sample.getMedium(layer).ua / sample.getMedium(layer).ut; // OVERFLOW BIN
    else
        A(iz, ir) += photon.weight * sample.getMedium(layer).ua / sample.getMedium(layer).ut;

    photon.weight *= sample.getMedium(layer).us / sample.getMedium(layer).ut;
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Spin(Photon<T>& photon) {
    int layer = photon.layer;
    T g = sample.getMedium(layer).g;
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

template < typename T, size_t Nz, size_t Nr >
bool MonteCarlo<T, Nz, Nr>::HitBoundary(Photon<T>& photon){
    bool hit;
    int layer = photon.layer;
    T uz = photon.direction.z;
    T mT = sample.getMedium(layer).ut;

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

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::CrossOrNot(Photon<T>& photon){
    if (photon.direction.z < 0.0)
        CrossUpOrNot(photon);
    else
        CrossDownOrNot(photon);
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::CrossUpOrNot(Photon<T>& photon){
    if (debug && photon.number == debugPhoton) {
            std::cout << "CrossUpOrNot" << std::endl;
    }
    T cosi = photon.direction.z;
    int layer = photon.layer;
    T ni = sample.getMedium(layer).n;
    T nt = 1.0;
    if (layer == 0)
        nt = sample.getNvacUpper();
    else
        nt = sample.getMedium(layer - 1).n;
    T cost = CosT(ni, nt, cosi);
    T Ri = FresnelR(ni, nt, cosi);
    T RND = random(0.0, 1.0); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton) {
            std::cout << "cost = " << cost << " FresnelR = "<< Ri <<" RND = "<< RND << std::endl;
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
        photon.direction.x *= ni / nt;
        photon.direction.y *= ni / nt;
        photon.direction.z = cost;
    } else { // fully reflected
        if (debug && photon.number == debugPhoton) {
            std::cout << "REFLECTED FROM BND" << std::endl;
        }
        photon.direction.z = -photon.direction.z;
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::CrossDownOrNot(Photon<T>& photon){
    if (debug && photon.number == debugPhoton) {
            std::cout << "CrossDownOrNot" << std::endl;
        }
    T cosi = photon.direction.z;
    int layer = photon.layer;
    T ni = sample.getMedium(layer).n;
    T nt = 1.0;
    if (layer == sample.getNlayers() - 1)
        nt = sample.getNvacLower();
    else
        nt = sample.getMedium(layer + 1).n;
    T cost = CosT(ni, nt, cosi);
    T Ri = FresnelR(ni, nt, cosi);
    T RND = random(0.0, 1.0); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton) {
            std::cout << "cost = " << cost << " FresnelR = "<< Ri << " RND = "<< RND << std::endl;
        }

    if (layer == (sample.getNlayers() - 1) && Ri < 1) { // partially transmitted -- only on sample border
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
        photon.direction.x *= ni / nt;
        photon.direction.y *= ni / nt;
        photon.direction.z = cost;
    } else {// fully reflected
        if (debug && photon.number == debugPhoton) {
            std::cout << "REFLECTED FROM BND" << std::endl;
        }
        photon.direction.z = -photon.direction.z;
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Roulette(Photon<T>& photon) {
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
    photon = Photon<T>(startCoord, startDir, 1.0, num);
    FirstReflection(photon);
    if (sample.getMedium(0).ut == 0) { // 1st layer is glass -- go directly to tissue
        photon.layer = 1;
        photon.coordinate.z = sample.CurrentUpperBorderZ(photon.layer);
    }
    while(photon.alive) {
       HopDropSpin(photon);
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

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr >::printResults() {
    std::cout << "Diffuse reflection = " << results.diffuseReflection << std::endl;
    std::cout << "Specular reflection = " << results.specularReflection << std::endl;
    std::cout << "Diffuse transmission = " << results.diffuseTransmission << std::endl;
    std::cout << "Absorbed fraction = " << results.absorbed << std::endl;
}
