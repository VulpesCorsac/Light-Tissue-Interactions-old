#pragma once

#include "Photon.h"
#include "Medium.h"
#include "Fresnel.h"
#include "Sample.h"
#include "Detector.h"
#include "BugerLambert.h"

#include "../Utils/Random.h"

#include "../eigen/Eigen/Dense"

#include <cmath>
#include <iostream>
#include <tgmath.h>

using namespace Eigen;

template < typename T, size_t Nz, size_t Nr, bool detector >
struct MCresults {
    T specularReflection = 0;
    T diffuseReflection = 0;
    T diffuseTransmission = 0;
    T absorbed = 0;
    T BugerTransmission = 0;

    Matrix<T,Dynamic,Dynamic> matrixA = Matrix<T, Nz, Nr>::Constant(0);
    Matrix<T,1,Dynamic> arrayR = Matrix<T, 1, Nr>::Constant(0);
    Matrix<T,1,Dynamic> arrayRspecular = Matrix<T, 1, Nr>::Constant(0);
    Matrix<T,1,Dynamic> arrayT = Matrix<T, 1, Nr>::Constant(0);

    std::vector<IntegratingSphere<T>> SpheresArrayR;
    std::vector<IntegratingSphere<T>> SpheresArrayT;

    std::vector<OpticalFiber<T>> FibersArrayR;
    std::vector<OpticalFiber<T>> FibersArrayT;

    std::vector<std::pair<T,T>> detectedR;
    std::vector<std::pair<T,T>> detectedT;

};

template < typename T, size_t Nz, size_t Nr, bool detector >
std::ostream& operator << (std::ostream& os, const MCresults<T,Nz,Nr,detector>& results) noexcept {
    using namespace std;

    os << "Diffuse reflection = "   << results.diffuseReflection   << endl;
    os << "Specular reflection = "  << results.specularReflection  << endl;
    os << "Diffuse transmission = " << results.diffuseTransmission << endl;
    os << "Absorbed fraction = "    << results.absorbed            << endl;
    os << "Unscattered transmission = " << results.BugerTransmission << endl;
    os << "Detected R" << endl;
    for (int i = 0; i < results.detectedR.size(); i++)
        os << results.detectedR[i].first << " " << results.detectedR[i].second << endl;
    os << "Detected T" << endl;
    for (int i = 0; i < results.detectedT.size(); i++)
        os << results.detectedT[i].first << " " << results.detectedT[i].second << endl;

    return os;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
class MonteCarlo {
public:
    MonteCarlo() noexcept = delete;
    MonteCarlo(const Sample<T>& new_sample, const int& new_Np, const T& new_z, const T& new_r, const IntegratingSphere<T>& new_sphereR, const IntegratingSphere<T>& new_sphereT, const DetectorDistances<T> new_dist);
    MonteCarlo(const Sample<T>& new_sample, const int& new_Np, const T& new_z, const T& new_r, const OpticalFiber<T>& new_fiberR, const OpticalFiber<T>& new_fiberT, const DetectorDistances<T> new_dist);
    ~MonteCarlo() noexcept = default;

    /// TODO: Why not return result?
    void Calculate(MCresults<T,Nz,Nr,detector>& res);
    MCresults<T,Nz,Nr,detector> CalculateResult();

    inline Matrix<T,Dynamic,Dynamic> getMatrixA() const noexcept { return A; }
    inline Matrix<T,Dynamic,Dynamic> getArrayR() const noexcept { return RR; }
    inline Matrix<T,Dynamic,Dynamic> getArrayRspec() const noexcept { return RRspecular; }
    inline Matrix<T,Dynamic,Dynamic> getArrayT() const noexcept { return TT; }

protected:
    const Sample<T>& sample;

    const int Nphotons;

    const T dz;
    const T dr;
    const T chance;
    const T threshold;

    bool debug = 0;
    int debugPhoton = 4;

    Matrix<T,Dynamic,Dynamic> A = Matrix<T, Nz, Nr>::Constant(0.0);
    Matrix<T,1,Dynamic> RR = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T,1,Dynamic> RRspecular = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T,1,Dynamic> TT = Matrix<T, 1, Nr>::Constant(0.0);

    IntegratingSphere<T> mainSphereR;
    IntegratingSphere<T> mainSphereT;
    std::vector<IntegratingSphere<T>> SpheresArrayR;
    std::vector<IntegratingSphere<T>> SpheresArrayT;

    OpticalFiber<T> mainFiberR;
    OpticalFiber<T> mainFiberT;
    std::vector<OpticalFiber<T>> FibersArrayR;
    std::vector<OpticalFiber<T>> FibersArrayT;

    DetectorDistances<T> distances;

    void GenerateDetectorArrays ();
    void PhotonDetectionSphereR (Photon<T>& exit_photon);
    void PhotonDetectionSphereT (Photon<T>& exit_photon);

    void FirstReflection(Photon<T>& photon);

    void HopDropSpin(Photon<T>& photon);
    void HopInGlass(Photon<T>& photon);
    void HopDropSpinInTissue(Photon<T>& photon);

    void StepSizeInGlass(Photon<T>& photon);
    void StepSizeInTissue(Photon<T>& photon);

    void RecordR(Photon<T>& photon, const T& FRefl, const T& cosT);
    void RecordT(Photon<T>& photon, const T& FRefl, const T& cosT);

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

    MCresults<T,Nz,Nr,detector> results;
};

template < typename T, size_t Nz, size_t Nr, bool detector>
MonteCarlo<T,Nz,Nr,detector>::MonteCarlo(const Sample<T>& new_sample, const int& new_Np, const T& new_z, const T& new_r, const IntegratingSphere<T>& new_detectorR, const IntegratingSphere<T>& new_detectorT, const DetectorDistances<T> new_dist)
    : sample(new_sample)
    , Nphotons(new_Np)
    , dz(new_z / Nz)
    , dr(new_r / Nr)
    , chance(0.1)
    , threshold(1e-4)
    , mainSphereR(new_detectorR)
    , mainSphereT(new_detectorT)
    , distances(new_dist) {
        GenerateDetectorArrays();
}

template < typename T, size_t Nz, size_t Nr, bool detector>
MonteCarlo<T,Nz,Nr,detector>::MonteCarlo(const Sample<T>& new_sample, const int& new_Np, const T& new_z, const T& new_r, const OpticalFiber<T>& new_detectorR, const OpticalFiber<T>& new_detectorT, const DetectorDistances<T> new_dist)
    : sample(new_sample)
    , Nphotons(new_Np)
    , dz(new_z / Nz)
    , dr(new_r / Nr)
    , chance(0.1)
    , threshold(1e-4)
    , mainFiberR(new_detectorR)
    , mainFiberT(new_detectorT)
    , distances(new_dist) {
        GenerateDetectorArrays();
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::GenerateDetectorArrays () {
    using namespace std;
    int length;
    if (distances.maxDist == distances.minDist)
        length = 1;
    else
        length = (distances.maxDist - distances.minDist) / distances.stepSize + 1;
    for(int i = 0; i < length; i++) {
        if (detector == 1) {
            SpheresArrayR.push_back(IntegratingSphere<T>(mainSphereR, distances.minDist + i*distances.stepSize));
            SpheresArrayT.push_back(IntegratingSphere<T>(mainSphereT, distances.minDist + i*distances.stepSize));
        }
        /* else {
            FibersArrayR.push_back(OpticalFiber<T>(mainFiberR, distances.minDist + i*distances.stepSize));
            FibersArrayT.push_back(OpticalFiber<T>(mainFiberT, distances.minDist + i*distances.stepSize));
        }
        //*/
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::PhotonDetectionSphereR (Photon<T>& exit_photon) {
    for (int i = 0; i < SpheresArrayR.size(); i++) {
        T step = abs((SpheresArrayR[i].getDistance() - abs(exit_photon.coordinate.z))/ exit_photon.direction.z);
        exit_photon.coordinate += step * exit_photon.direction;
        if (debug && exit_photon.number == debugPhoton)
            std::cout << exit_photon << std::endl;
        if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereR.getDPort1() / 2)) {
            T stepSphere = abs(mainSphereR.getDSphere()/ exit_photon.direction.z);
            exit_photon.coordinate += stepSphere * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                std::cout << exit_photon << std::endl;
            if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) >= sqr(mainSphereR.getDPort2() /2)) {
                SpheresArrayR[i].totalLight += exit_photon.weight;
                if (debug && exit_photon.number == debugPhoton)
                    std::cout << "caught by R sphere " << i << std::endl;
            }
            exit_photon.coordinate -= stepSphere * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                std::cout << exit_photon << std::endl;
        } else
            continue;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::PhotonDetectionSphereT (Photon<T>& exit_photon) {
    for (int i = 0; i < SpheresArrayT.size(); i++) {
        T step = abs(((SpheresArrayT[i].getDistance() + sample.getTotalThickness()) - exit_photon.coordinate.z)/ exit_photon.direction.z);
         exit_photon.coordinate += step * exit_photon.direction;
         if (debug && exit_photon.number == debugPhoton)
            std::cout << exit_photon << std::endl;
        if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereT.getDPort1() / 2)) {
            T stepSphere = abs(mainSphereT.getDSphere() / exit_photon.direction.z);
            exit_photon.coordinate += stepSphere * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                std::cout << exit_photon << std::endl;
            if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) >= sqr(mainSphereT.getDPort2() / 2)) {
                SpheresArrayT[i].totalLight += exit_photon.weight;
                if (debug && exit_photon.number == debugPhoton)
                    std::cout << "caught by T sphere " << i << std::endl;
            }
            exit_photon.coordinate -= stepSphere * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                std::cout << exit_photon << std::endl;
        } else
            break;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::FirstReflection(Photon<T>& photon) {
    using namespace std;

    const auto ni = sample.getNvacUpper();
    const auto nt = sample.getMedium(0).n;
    const auto cosi = abs(photon.direction.z);
    const auto cost = CosT(ni, nt, cosi);

    T Ri = FresnelR(ni, nt, cosi);

    if (sample.getMedium(0).ut == 0) { // specular from glass
        const auto R1 = FresnelR(ni, nt, cosi);
        const auto n3 = sample.getMedium(1).n;
        const auto R2 = FresnelR(nt, n3, cost);
        Ri = R1 + (sqr(1 - R1) * R2) / (1 - R1 * R2);
    }

    const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const size_t ir = floor(r/dr);

    RRspecular(ir) += Ri * photon.weight;

    auto exitCoord = photon.coordinate; // this is for normal incidence only!
    auto exitDir = Vector3D<T>(photon.direction.x, photon.direction.y, -photon.direction.z);
    auto exitWeight = Ri * photon.weight;
    Photon<T> exitPhoton = Photon<T>(exitCoord, exitDir, exitWeight, photon.number);
    PhotonDetectionSphereR(exitPhoton);

    photon.weight *= (1 - Ri);
    photon.direction.z = CosT(ni, nt, cosi);
    // photon.coordinate.z += 1e-9; // crook
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::HopDropSpin(Photon<T>& photon) {
    if (sample.getMedium(photon.layer).ut == 0)
        HopInGlass(photon);
    else
        HopDropSpinInTissue(photon);
    Roulette(photon);
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::HopInGlass(Photon<T>& photon) {
    using namespace std;

    if (debug && photon.number == debugPhoton) {
        cout << "Before HopInGlass" << endl;
        cout << photon << endl;
    }

    if (photon.direction.z == 0)
        photon.alive = 0;
    else {
        StepSizeInGlass(photon);
        Hop(photon);
        CrossOrNot(photon);
    }

    if (debug && photon.number == debugPhoton) {
        cout << "After HopInGlass" << endl;
        cout << photon << endl;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::HopDropSpinInTissue(Photon<T>& photon) {
    using namespace std;

    StepSizeInTissue(photon);
    if (HitBoundary(photon)) {
        if (debug && photon.number == debugPhoton) {
            cout << "Before HopInTissue" << endl;
            cout << photon << endl;
        }
        Hop(photon);
        CrossOrNot(photon);
        if (debug && photon.number == debugPhoton) {
            cout << "After HopInTissue" << endl;
            cout << photon << endl;
        }
    } else {
        if (debug && photon.number == debugPhoton) {
            cout << "Before Hop" << endl;
            cout << photon << endl;
        }
        Hop(photon);
        if (debug && photon.number == debugPhoton) {
            cout << "After Hop" << endl;
            cout << photon << endl;
        }
        Drop(photon);
        if (debug && photon.number == debugPhoton) {
            cout << "After Drop" << endl;
            cout << photon << endl;
        }
        Spin(photon);
        if (debug && photon.number == debugPhoton) {
            cout << "After Spin" << endl;
            cout << photon << endl;
        }
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::StepSizeInGlass(Photon<T>& photon) {
    const auto uz = photon.direction.z;
    photon.step = 0;
    if (uz > 0)
        photon.step = (sample.CurrentLowerBorderZ(photon.layer) - photon.coordinate.z) / uz;
    else if (uz < 0)
        photon.step = (sample.CurrentUpperBorderZ(photon.layer) - photon.coordinate.z) / uz;
    /// TODO: doesn't it freeze if uz == 0?
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::StepSizeInTissue(Photon<T>& photon) {
    using namespace std;

    const auto mT = sample.getMedium(photon.layer).ut;
    if (photon.stepLeft == 0) // new step
        photon.step = -log(random<T>(0, 1)) / mT;
    else { // leftover step
        photon.step = photon.stepLeft / mT;
        photon.stepLeft = 0;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::RecordR(Photon<T>& photon, const T& FRefl, const T& cosT) {
    using namespace std;

    const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const size_t ir = floor(r/dr);

    RR(min(ir, Nr-1)) += (1 - FRefl) * photon.weight;

    if (debug && photon.number == debugPhoton) {
        cout << "RR at " << ir << " = " << RR(ir) << endl;
        cout << photon << endl;
    }

    auto exitCoord = photon.coordinate;
    auto exitDir = Vector3D<T>(photon.direction.x, photon.direction.y, cosT);
    auto exitWeight = (1 - FRefl) * photon.weight;
    Photon<T> exitPhoton = Photon<T>(exitCoord, exitDir, exitWeight, photon.number);
    PhotonDetectionSphereR(exitPhoton);

    photon.weight *= FRefl;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::RecordT(Photon<T>& photon, const T& FRefl, const T& cosT) {
    using namespace std;

    const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const size_t ir = floor(r/dr);

    TT(min(ir, Nr-1)) += (1 - FRefl) * photon.weight;

    if (debug && photon.number == debugPhoton) {
        cout << "TT at " << ir << " = " << TT(ir) << endl;
        cout << photon << endl;
    }

    auto exitCoord = photon.coordinate;
    auto exitDir = Vector3D<T>(photon.direction.x, photon.direction.y, cosT);
    auto exitWeight = (1 - FRefl) * photon.weight;
    Photon<T> exitPhoton = Photon<T>(exitCoord, exitDir, exitWeight, photon.number);
    PhotonDetectionSphereT(exitPhoton);

    photon.weight *= FRefl;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::Hop(Photon<T>& photon) {
    photon.coordinate += photon.step * photon.direction;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::Drop(Photon<T>& photon) {
    using namespace std;

    const int layer = photon.layer;
    const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const size_t ir = floor(r / dr);
    const size_t iz = abs(floor(photon.coordinate.z / dz));

    if (iz >= Nz)
        cout << "ACHTUNG!!! iz = " << iz << " exceeds Nz during drop of photon N " << photon.number << endl;

    A(iz, min(ir, Nr-1)) += photon.weight * sample.getMedium(layer).ua / sample.getMedium(layer).ut;

    photon.weight *= sample.getMedium(layer).us / sample.getMedium(layer).ut;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::Spin(Photon<T>& photon) {
    using namespace std;

    const int layer = photon.layer;
    const T g = sample.getMedium(layer).g;

    const auto RND1 = random<T>(0, 1);
    T cosHG = (1 + sqr(g) - sqr((1 - sqr(g)) / (1 - g + 2 * g * RND1))) / (2 * g);
    if (g == 0)
        cosHG = 2 * RND1 - 1;
    else if (g == 1)
        cosHG = 1;

    const auto RND2 = random<T>(0, 1);
    const auto phi = 2 * M_PI * RND2; // radians

    if (debug && photon.number == debugPhoton)
        cout << "RND1 = " << RND1 << " cosHG = " << cosHG << " RND2 = " << RND2 << " phi = " << phi << endl;

    T ux = photon.direction.x;
    T uy = photon.direction.y;
    T uz = photon.direction.z;

    const auto sinHG = sqrt(1 - sqr(cosHG));
    const auto temp = sqrt(1 - sqr(uz));

    T uxx = +sinHG * (ux * uz * cos(phi) - uy * sin(phi)) / temp + ux * cosHG;
    T uyy = +sinHG * (uy * uz * cos(phi) + ux * sin(phi)) / temp + uy * cosHG;
    T uzz = -sinHG *            cos(phi)                  * temp + uz * cosHG;

    if (abs(abs(uz) - 1)  < 1e-6) {
        uxx = sinHG * cos(phi);
        uyy = sinHG * sin(phi);
        uzz = uz >= 0 ? cosHG : -cosHG;
    }

    photon.direction.x = uxx;
    photon.direction.y = uyy;
    photon.direction.z = uzz;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
bool MonteCarlo<T,Nz,Nr,detector>::HitBoundary(Photon<T>& photon) {
    const auto uz = photon.direction.z;

    T distToBnd = 0;
    if (uz > 0)
        distToBnd = (sample.CurrentLowerBorderZ(photon.layer) - photon.coordinate.z) / uz;
    else if (uz < 0)
        distToBnd = (sample.CurrentUpperBorderZ(photon.layer) - photon.coordinate.z) / uz;

    if (uz != 0 && photon.step > distToBnd) {
        photon.stepLeft = (photon.step - distToBnd) * sample.getMedium(photon.layer).ut;
        photon.step = distToBnd;
        return true;
    }
    return false;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::CrossOrNot(Photon<T>& photon) {
    return photon.direction.z < 0 ? CrossUpOrNot(photon) : CrossDownOrNot(photon);
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::CrossUpOrNot(Photon<T>& photon) {
    using namespace std;

    if (debug && photon.number == debugPhoton)
        cout << "CrossUpOrNot" << endl;

    const T cosi = photon.direction.z;
    const int layer = photon.layer;
    const auto ni = sample.getMedium(layer).n;
    const auto nt = layer == 0 ? sample.getNvacUpper() : sample.getMedium(layer - 1).n;
    const auto cost = CosT(ni, nt, cosi);
    const auto Ri = FresnelR(ni, nt, cosi);
    const auto RND = random<T>(0, 1); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton)
        cout << "cost = " << cost << " FresnelR = " << Ri << " RND = " << RND << endl;

    if (layer == 0 && Ri < 1) { // partially transmitted -- only on sample border
        if (debug && photon.number == debugPhoton)
            cout << "SAMPLE BORDER" << endl;
        RecordR(photon, Ri, cost);
        photon.direction.z = - photon.direction.z;
    } else if (RND > Ri) { // fully transmitted
        if (debug && photon.number == debugPhoton)
            cout << "TRANSMITTED THROUGH BND" << endl;
        photon.layer -= 1;
        photon.direction.x *= ni / nt;
        photon.direction.y *= ni / nt;
        photon.direction.z = cost;
    } else { // fully reflected
        if (debug && photon.number == debugPhoton)
            cout << "REFLECTED FROM BND" << endl;
        photon.direction.z = -photon.direction.z;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::CrossDownOrNot(Photon<T>& photon) {
    using namespace std;

    if (debug && photon.number == debugPhoton)
        cout << "CrossDownOrNot" << endl;
    const auto cosi = photon.direction.z;
    const int layer = photon.layer;
    const auto ni = sample.getMedium(layer).n;
    const auto nt = layer == sample.getNlayers()-1 ? sample.getNvacLower() : sample.getMedium(layer + 1).n;
    const auto cost = CosT(ni, nt, cosi);
    const auto Ri = FresnelR(ni, nt, cosi);
    const auto RND = random<T>(0, 1); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton)
        cout << "cost = " << cost << " FresnelR = "<< Ri << " RND = "<< RND << endl;

    if (layer == (sample.getNlayers() - 1) && Ri < 1) { // partially transmitted -- only on sample border
        if (debug && photon.number == debugPhoton)
            cout << "SAMPLE BORDER" << endl;
        RecordT(photon, Ri, cost);
        photon.direction.z *= -1;
    } else if (RND > Ri) { // fully transmitted
        if (debug && photon.number == debugPhoton)
            cout << "TRANSMITTED THROUGH BND" << endl;
        photon.layer += 1;
        photon.direction.x *= ni / nt;
        photon.direction.y *= ni / nt;
        photon.direction.z = cost;
    } else { // fully reflected
        if (debug && photon.number == debugPhoton)
            cout << "REFLECTED FROM BND" << endl;
        photon.direction.z *= -1;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::Roulette(Photon<T>& photon) {
    using namespace std;

    if (photon.weight < threshold) {
        const auto RND = random<T>(0, 1);
        if (debug && photon.number == debugPhoton)
            cout << "Kill? RND = " << RND << endl;
        if (RND <= chance)
            photon.weight /= chance;
        else
            photon.alive = false;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::Simulation(Photon<T>& photon, const int& num) {
    const auto startCoord = Vector3D<T>(0, 0, 0);
    const auto startDir = Vector3D<T>(0, 0, 1); // normal incidence for now
    photon = Photon<T>(startCoord, startDir, 1.0, num);
    FirstReflection(photon);
    if (sample.getMedium(0).ut == 0) { // 1st layer is glass -- go directly to tissue
        photon.layer = 1;
        photon.coordinate.z = sample.CurrentUpperBorderZ(photon.layer);
    }

    while(photon.alive)
       HopDropSpin(photon);
}

template < typename T, size_t Nz, size_t Nr, bool detector>
T MonteCarlo<T,Nz,Nr,detector>::Volume(const T& ir) {
    return Area(ir) * dz;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
T MonteCarlo<T,Nz,Nr,detector>::Area(const T& ir) {
    return 2 * M_PI * (ir - 0.5) * sqr(dr);
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T, Nz, Nr, detector >::Calculate(MCresults<T,Nz,Nr,detector>& res) {
    for (int i = 0; i < Nphotons; i++) {
        Photon<T> myPhoton;
        Simulation(myPhoton, i);
        // cout << RRspecular(0) << endl;
    }

    results.arrayR = RR;
    results.arrayRspecular = RRspecular;
    results.arrayT = TT;
    results.matrixA = A;

    results.diffuseReflection = RR.sum() / Nphotons;
    results.specularReflection = RRspecular.sum() / Nphotons;
    results.diffuseTransmission = TT.sum() / Nphotons;
    results.absorbed = A.sum() / Nphotons;

    if (sample.getNlayers() == 1)
        results.BugerTransmission = BugerLambert(sample.getMedium(0).tau, sample.getMedium(0).n, sample.getNvacLower(), sample.getNvacLower());
    else
        results.BugerTransmission = BugerLambert(sample.getMedium(1).tau, sample.getMedium(1).n, sample.getMedium(0).n, sample.getMedium(2).n);

    if (detector == 1) {
        results.SpheresArrayR = SpheresArrayR;
        results.SpheresArrayT = SpheresArrayT;
        for (int i = 0; i < SpheresArrayR.size(); i++) {
            results.detectedR.push_back(std::make_pair(SpheresArrayR[i].getDistance(), SpheresArrayR[i].totalLight / Nphotons));
            results.detectedT.push_back(std::make_pair(SpheresArrayT[i].getDistance(), SpheresArrayT[i].totalLight / Nphotons));
        }
    }
    /*
    else {
        for (int i = 0; i < FibersArrayR.size(); i++) {
            results.detectedR.push_back(std::make_pair(FibersArrayR[i].getDistance(), FibersArrayR[i].totalLight / Nphotons));
            results.detectedT.push_back(std::make_pair(FibersArrayT[i].getDistance(), FibersArrayT[i].totalLight / Nphotons));
        }
    }
    //*/

    res = results;

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

template < typename T, size_t Nz, size_t Nr, bool detector>
MCresults<T,Nz,Nr,detector> MonteCarlo<T, Nz, Nr, detector >::CalculateResult() {
    MCresults<T,Nz,Nr,detector> res;
    Calculate(res);
    return res;
}
