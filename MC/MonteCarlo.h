#pragma once

#include "Detector.h"
#include "Medium.h"
#include "Photon.h"
#include "Sample.h"
#include "LightSource.h"

#include "../Math/Basic.h"
#include "../Math/Random.h"
#include "../Math/Bresenham.h"
#include "../Physics/BugerLambert.h"
#include "../Physics/Reflectance.h"
#include "../Utils/Utils.h"

#include "../eigen/Eigen/Dense"

#include <iostream>
#include <math.h>
#include <tgmath.h>
#include <map>
#include <chrono>

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
    Matrix<T,Dynamic,Dynamic> heatSource = Matrix<T, Nz, Nr>::Constant(0);
    Matrix<T,Dynamic,Dynamic> heatSourceNorm = Matrix<T, Nz, Nr>::Constant(0);

    Matrix<T,1,Dynamic> arrayAnglesR = Matrix<T, 1, 100>::Constant(0);
    Matrix<T,1,Dynamic> arrayAnglesT = Matrix<T, 1, 100>::Constant(0);

    std::vector<IntegratingSphere<T>> SpheresArrayR;
    std::vector<IntegratingSphere<T>> SpheresArrayT;

    IntegratingSphere<T> mainSphereR;
    IntegratingSphere<T> mainSphereT;
    LightSource<T> lightSource;
    std::vector<std::pair<T,T>> sourceMatrix;

    //std::vector<OpticalFiber<T>> FibersArrayR;
    //std::vector<OpticalFiber<T>> FibersArrayT;

    /// TODO: structs instead of pair
    std::vector<std::pair<T,T>> detectedR;
    std::vector<std::pair<T,T>> detectedT;
};

template < typename T, size_t Nz, size_t Nr, bool detector >
std::ostream& operator << (std::ostream& os, const MCresults<T,Nz,Nr,detector>& results) noexcept {
    os << "Diffuse reflection = "       << results.diffuseReflection   << '\n';
    os << "Specular reflection = "      << results.specularReflection  << '\n';
    os << "Diffuse transmission = "     << results.diffuseTransmission << '\n';
    os << "Absorbed fraction = "        << results.absorbed            << '\n';
    os << "Unscattered transmission = " << results.BugerTransmission   << '\n';

    os << "Detected R" << '\n';
    for (const auto& x: results.detectedR)
        os << x.first << " " << x.second << '\n';
    os << "Detected T" << '\n';
    for (const auto& x: results.detectedT)
        os << x.first << " " << x.second << '\n';

    return os;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
class MonteCarlo {
public:
    MonteCarlo() noexcept = delete;
    MonteCarlo(const Sample<T>& sample, const int& Np, const T& z, const T& r, const IntegratingSphere<T>& sphereR, const IntegratingSphere<T>& sphereT,
               const DetectorDistance<T> dist, const LightSource<T>& source);
    MonteCarlo(const Sample<T>& sample, const int& Np, const T& z, const T& r, const IntegratingSphere<T>& sphereR, const IntegratingSphere<T>& sphereT,
               const DetectorDistance<T> dist, const LightSource<T>& source, const Matrix<T,Dynamic,Dynamic>& coagMatrix);
    //MonteCarlo(const Sample<T>& sample, const int& Np, const T& z, const T& r, const OpticalFiber<T>& fiberR, const OpticalFiber<T>& fiberT, const DetectorDistance<T> dist);
    ~MonteCarlo() noexcept = default;

    /// TODO: Why not return result?
    void Calculate(MCresults<T,Nz,Nr,detector>& res);
    MCresults<T,Nz,Nr,detector> CalculateResult();

    inline Matrix<T,Dynamic,Dynamic> getMatrixA()    const noexcept { return A;            }
    inline Matrix<T,Dynamic,Dynamic> getArrayR()     const noexcept { return RR;           }
    inline Matrix<T,Dynamic,Dynamic> getArrayRspec() const noexcept { return RRspecular;   }
    inline Matrix<T,Dynamic,Dynamic> getArrayT() 	 const noexcept { return TT;           }
    inline Matrix<T,Dynamic,Dynamic> getAnglesR() 	 const noexcept { return arrayAnglesR; }
    inline Matrix<T,Dynamic,Dynamic> getAnglesT()    const noexcept { return arrayAnglesT; }

protected:
    const Sample<T>& sample;
    const LightSource<T>& lightSource;

    const int Nphotons;

    const T dx, dy, dz;
    const T dr;
    const T chance;
    const T threshold;
    const T radius;

    bool debug = 0;
    int debugPhoton = 0;

    Matrix<T,Dynamic,Dynamic> A = Matrix<T, Nz, Nr>::Constant(0.0);
    Matrix<T,1,Dynamic> RR = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T,1,Dynamic> RRspecular = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T,1,Dynamic> TT = Matrix<T, 1, Nr>::Constant(0.0);

    Matrix<T,1,Dynamic> arrayAnglesR = Matrix<T, 1, 100>::Constant(0);
    Matrix<T,1,Dynamic> arrayAnglesT = Matrix<T, 1, 100>::Constant(0);

    IntegratingSphere<T> mainSphereR;
    IntegratingSphere<T> mainSphereT;
    std::vector<IntegratingSphere<T>> SpheresArrayR;
    std::vector<IntegratingSphere<T>> SpheresArrayT;

    //OpticalFiber<T> mainFiberR;
    //OpticalFiber<T> mainFiberT;
    //std::vector<OpticalFiber<T>> FibersArrayR;
    //std::vector<OpticalFiber<T>> FibersArrayT;

    DetectorDistance<T> distances;

    const bool homogenous;
    const Matrix<T,Dynamic,Dynamic> heterogeneousMatrix2D;
    const std::vector<Matrix<T,Dynamic,Dynamic>> heterogeneousMatrix3D;
    std::vector<Matrix<T, Dynamic, Dynamic>> getHeterogeneousMatrix3D();

    void GenerateDetectorArrays();
    void PhotonDetectionSphereR(Photon<T>& exit_photon);
    void PhotonDetectionSphereT(Photon<T>& exit_photon);

    void FirstReflection(Photon<T>& photon);

    void HopDropSpin(Photon<T>& photon);
    void HopInGlass(Photon<T>& photon);
    void HopDropSpinInTissue(Photon<T>& photon);
    void HopDropSpinInHeterogeneousTissue(Photon<T>& photon);

    std::vector<Vector3D<int>> TrajectoryArrayInt(Photon<T>& photon, Vector3D<T>& finalBorderPoint);
    void InnerBordersArray(Photon<T>& photon, std::vector<Vector3D<T>>& bordersArray, std::vector<T>& attCoeffs);
    void HopInHeterogeneousTissue(Photon<T>& photon, const std::vector<Vector3D<T>>& bordersArray, const std::vector<T>& attCoeffs);
    void HopInHeterogeneousTissueNoBorder(Photon<T>& photon);

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
    Vector3D<int> CartesianGridPoint(const Vector3D<T>& point);
    Vector3D<T> CartesianCoord(const Vector3D<int>& point);

    MCresults<T,Nz,Nr,detector> results;
};

template < typename T, size_t Nz, size_t Nr, bool detector>
MonteCarlo<T,Nz,Nr,detector>::MonteCarlo(const Sample<T>& newSample, const int& Np, const T& z, const T& r,
                                         const IntegratingSphere<T>& detectorR, const IntegratingSphere<T>& detectorT,
                                         const DetectorDistance<T> dist, const LightSource<T>& source)
    : sample(newSample)
    , Nphotons(Np)
    , dx(2 * r / (2 * Nr - 1))
    , dy(2 * r / (2 * Nr - 1))
    , dz(z / Nz)
    , dr(r / Nr)
    , chance(0.1)
    , threshold(1E-4)
    , mainSphereR(detectorR)
    , mainSphereT(detectorT)
    , distances(dist)
    , lightSource(source)
    , radius(r)
    , homogenous(1) {
    GenerateDetectorArrays();
}

template < typename T, size_t Nz, size_t Nr, bool detector>
MonteCarlo<T,Nz,Nr,detector>::MonteCarlo(const Sample<T>& newSample, const int& Np, const T& z, const T& r,
                                         const IntegratingSphere<T>& detectorR, const IntegratingSphere<T>& detectorT,
                                         const DetectorDistance<T> dist, const LightSource<T>& source, const Matrix<T,Dynamic,Dynamic>& coagMatrix)
    : sample(newSample)
    , Nphotons(Np)
    , dx(2 * r / (2 * Nr - 1))
    , dy(2 * r / (2 * Nr - 1))
    , dz(z / Nz)
    , dr(r / Nr)
    , chance(0.1)
    , threshold(1E-4)
    , mainSphereR(detectorR)
    , mainSphereT(detectorT)
    , distances(dist)
    , lightSource(source)
    , radius(r)
    , heterogeneousMatrix2D(coagMatrix)
    , heterogeneousMatrix3D(getHeterogeneousMatrix3D())
    , homogenous(0) {
    GenerateDetectorArrays();
/*    cerr << coagMatrix << endl;
    cerr << endl;
    cerr << heterogeneousMatrix2D << endl;
    cerr << endl;
    for (auto x : heterogeneousMatrix3D) {
        cerr << x << endl;
        cerr << endl;
    }*/
}

/*
template < typename T, size_t Nz, size_t Nr, bool detector>
MonteCarlo<T,Nz,Nr,detector>::MonteCarlo(const Sample<T>& sample, const int& Np, const T& z, const T& r, const OpticalFiber<T>& detectorR, const OpticalFiber<T>& detectorT, const DetectorDistance<T> dist)
    : sample(sample)
    , Nphotons(Np)
    , dz(z / Nz)
    , dr(r / Nr)
    , chance(0.1)
    , threshold(1E-4)
    , mainFiberR(detectorR)
    , mainFiberT(detectorT)
    , distances(dist) {
    GenerateDetectorArrays();
}
//*/
template < typename T, size_t Nz, size_t Nr, bool detector>
std::vector<Matrix<T, Dynamic, Dynamic>> MonteCarlo<T,Nz,Nr,detector>::getHeterogeneousMatrix3D() {
    using namespace std;
    using namespace Math_NS;
    std::vector<Matrix<T, Dynamic, Dynamic>> coag3D;
    for (int i = 0; i < Nz; i++) {
        Matrix<T,Dynamic,Dynamic> slice = Matrix<T, 2 * Nr - 1, 2 * Nr - 1>::Constant(1.0);
        Matrix<T,1,Dynamic> line = heterogeneousMatrix2D.row(i);
        Matrix<T,1,Dynamic> longLine = Matrix<T,1,2 * Nr - 1>::Zero();
        longLine << line.rowwise().reverse(), line(all, lastN(Nr-1));
  //      cerr << "longLine " << longLine << endl;
            for (int k = 0; k < 2 * Nr - 1; k++)
                for (int l = 0; l < 2 * Nr - 1; l++) {
                    if (sqr(k - Nr + 1) + sqr(l - Nr + 1) <= sqr(Nr)) {
                        T dist = sqrt(sqr(k - Nr + 1) + sqr(l - Nr + 1));
                        int num = floor(dist);
              //          cerr << k << " " << l << " " << Nr - 1 << " " << dist << " " << num << endl;
                        slice(k, l) = line(num);
                    }
                }

     /*   for (int j = 0; j < 2 * Nr - 1; j++) {
            for (int k = 0; k < 2 * Nr - 1; k++) {
                slice(k,j) = longLine(j);
            }
        }*/
 //       cerr << slice << endl;
       coag3D.push_back(slice);
    }
    return coag3D;
}


template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::GenerateDetectorArrays() {
    using namespace std;

    /// TODO: you need * 1.0 ?
    double length = distances.max == distances.min ?
        1 :
        ((static_cast<T>(distances.max) - static_cast<T>(distances.min)) / static_cast<T>(distances.step)) * 1.0 + 1;

    length = length + 0.5 - (length < 0 ? 1 : 0);
    int nLength = length;

    for (int i = 0; i < nLength; i++) {
        if (detector == 1) {
            SpheresArrayR.push_back(IntegratingSphere<T>(mainSphereR, distances.min + i*distances.step));
            SpheresArrayT.push_back(IntegratingSphere<T>(mainSphereT, distances.min + i*distances.step));
        }
        /*
        else {
            FibersArrayR.push_back(OpticalFiber<T>(mainFiberR, distances.min + i*distances.step));
            FibersArrayT.push_back(OpticalFiber<T>(mainFiberT, distances.min + i*distances.step));
        }
        //*/
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::PhotonDetectionSphereR(Photon<T>& exit_photon) {
    using namespace Math_NS;
    using namespace Utils_NS;
    using namespace std;

    /*
    for (int i = 0; i < isize(SpheresArrayR); i++) {
        T step = abs((SpheresArrayR[i].getDistance() - abs(exit_photon.coordinate.z))/ exit_photon.direction.z);
        exit_photon.coordinate += step * exit_photon.direction;
        if (debug && exit_photon.number == debugPhoton)
            cout << exit_photon << endl;
        if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereR.getDPort1() / 2)) {
            T stepSphere = abs(mainSphereR.getDSphere()/ exit_photon.direction.z);
            exit_photon.coordinate += stepSphere * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                cout << exit_photon << endl;
            if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) >= sqr(mainSphereR.getDPort2() /2)) {
                SpheresArrayR[i].totalLight += exit_photon.weight;
                if (debug && exit_photon.number == debugPhoton)
                    cout << "caught by R sphere " << i << endl;
            }
            exit_photon.coordinate -= stepSphere * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                cout << exit_photon << endl;
        } else
            break;
    }
    //*/
    if (exit_photon.direction.x == 0) {
        arrayAnglesR[0] += 0.0;
    } else {
        if (debug && exit_photon.number == debugPhoton)
            cerr << "R " << exit_photon.direction.z << " " << exit_photon.direction.x << endl;
        T theta = atan(exit_photon.direction.z / exit_photon.direction.x);
        size_t iTheta = 0;
        if (theta > 0)
            iTheta = floor(theta / (M_PI / 100));
        else if (theta < 0)
            iTheta = floor((M_PI + theta) / (M_PI / 100));
        arrayAnglesR[iTheta] += exit_photon.weight;
    }
    /// weird thorlabs sphere
    // cout << isize(SpheresArrayR) << endl;
    for (int i = 0; i < isize(SpheresArrayR); i++) {
        T step = abs((SpheresArrayR[i].getDistance() - abs(exit_photon.coordinate.z))/ exit_photon.direction.z);
        exit_photon.coordinate += step * exit_photon.direction;
        if (debug && exit_photon.number == debugPhoton)
            cerr << "to sphere \n" << exit_photon << endl;
        if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereR.getDPort1() / 2)) { // gets inside tunnel
            T stepDarkTunnel = abs(0.003 / exit_photon.direction.z); // black part of tunnel is 3mm long
            exit_photon.coordinate += stepDarkTunnel * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                cerr << "in tunnel \n"<< exit_photon << endl;
            if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereR.getDPort1() / 2)) {
                T stepLightTunnel = abs(0.009 / exit_photon.direction.z);
                exit_photon.coordinate += stepLightTunnel * exit_photon.direction;
                if (debug && exit_photon.number == debugPhoton)
                    cerr << "in light tunnel \n"<< exit_photon << endl;
                if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereR.getDPort1() / 2)) {
                    T stepSphere = abs(mainSphereR.getDSphere() / exit_photon.direction.z);
                    exit_photon.coordinate += stepSphere * exit_photon.direction;
                    if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) >= sqr(mainSphereR.getDPort2() / 2)) {
                        SpheresArrayR[i].totalLight += exit_photon.weight;
                        if (debug && exit_photon.number == debugPhoton)
                            cerr << "caught by R sphere \n" << exit_photon << endl;
                        exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
                        exit_photon.coordinate -= stepLightTunnel * exit_photon.direction;
                        exit_photon.coordinate -= stepSphere * exit_photon.direction;
                    } else {
                        T stepLightTunnel2 = abs(0.009 / exit_photon.direction.z);
                        exit_photon.coordinate += stepLightTunnel2 * exit_photon.direction;
                        if (debug && exit_photon.number == debugPhoton)
                            cerr << "in light tunnel2 \n"<< exit_photon << endl;
                        if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) >= sqr(mainSphereR.getDPort2() / 2)) {
                            SpheresArrayR[i].totalLight += 0.3 * exit_photon.weight;
                            if (debug && exit_photon.number == debugPhoton)
                                cerr << "caught by tunnel \n" << exit_photon << endl;
                            exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
                            exit_photon.coordinate -= stepLightTunnel * exit_photon.direction;
                            exit_photon.coordinate -= stepSphere * exit_photon.direction;
                            exit_photon.coordinate -= stepLightTunnel2 * exit_photon.direction;
                        } else {
                            exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
                            exit_photon.coordinate -= stepLightTunnel * exit_photon.direction;
                            exit_photon.coordinate -= stepSphere * exit_photon.direction;
                            exit_photon.coordinate -= stepLightTunnel2 * exit_photon.direction;
                        }
                    }
                } else {
                    SpheresArrayR[i].totalLight += 0.3 * exit_photon.weight;
                    if (debug && exit_photon.number == debugPhoton)
                        cerr << "caught by tunnel \n" << exit_photon << endl;
                    exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
                    exit_photon.coordinate -= stepLightTunnel * exit_photon.direction;
                }
            } else
                exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
        } else
            break;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::PhotonDetectionSphereT(Photon<T>& exit_photon) {
    using namespace Math_NS;
    using namespace Utils_NS;
    using namespace std;

    /*
    for (int i = 0; i < isize(SpheresArrayT); i++) {
        T step = abs(((SpheresArrayT[i].getDistance() + sample.getTotalThickness()) - exit_photon.coordinate.z)/ exit_photon.direction.z);
         exit_photon.coordinate += step * exit_photon.direction;
         if (debug && exit_photon.number == debugPhoton)
            cout << exit_photon << endl;
        if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereT.getDPort1() / 2)) {
            T stepSphere = abs(mainSphereT.getDSphere() / exit_photon.direction.z);
            exit_photon.coordinate += stepSphere * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                cout << exit_photon << endl;
            if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) >= sqr(mainSphereT.getDPort2() / 2)) {
                SpheresArrayT[i].totalLight += exit_photon.weight;
                if (debug && exit_photon.number == debugPhoton)
                    cout << "caught by T sphere " << i << endl;
            }
            exit_photon.coordinate -= stepSphere * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                cout << exit_photon << endl;
        } else
            break;
    }
    //*/
    if (exit_photon.direction.x == 0) {
        arrayAnglesT[0] += 0.0;
    } else {
        if (debug && exit_photon.number == debugPhoton)
            cerr << "T " << exit_photon.direction.z << " " << exit_photon.direction.x << endl;
        T theta = atan(exit_photon.direction.z / exit_photon.direction.x);
        size_t iTheta = 0;
        if (theta > 0)
            iTheta = floor(theta / (M_PI / 100));
        else if (theta < 0)
            iTheta = floor((M_PI + theta) / (M_PI / 100));
        arrayAnglesT[iTheta] += exit_photon.weight;
    }
    /// weird thorlabs spheres with long tunnel
    for (int i = 0; i < isize(SpheresArrayT); i++) {
        T step = abs(((SpheresArrayT[i].getDistance() + sample.getTotalThickness()) - exit_photon.coordinate.z)/ exit_photon.direction.z);
        exit_photon.coordinate += step * exit_photon.direction;
        if (debug && exit_photon.number == debugPhoton)
            cerr << "to sphere \n" << exit_photon << endl;

        if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereT.getDPort1() / 2)) { // gets inside tunnel

            T stepDarkTunnel = abs(0.003 / exit_photon.direction.z); // black part of tunnel is 3mm long
            exit_photon.coordinate += stepDarkTunnel * exit_photon.direction;
            if (debug && exit_photon.number == debugPhoton)
                cerr << "in tunnel \n"<< exit_photon << endl;
            if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereT.getDPort1() / 2)) {
                T stepLightTunnel = abs(0.009 / exit_photon.direction.z);
                exit_photon.coordinate += stepLightTunnel * exit_photon.direction;
                if (debug && exit_photon.number == debugPhoton)
                    cerr << "in light tunnel \n"<< exit_photon << endl;
                if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) < sqr(mainSphereT.getDPort1() / 2)) {
                    T stepSphere = abs(mainSphereT.getDSphere() / exit_photon.direction.z);
                    exit_photon.coordinate += stepSphere * exit_photon.direction;
                    if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) >= sqr(mainSphereT.getDPort2() / 2)) {
                        SpheresArrayT[i].totalLight += exit_photon.weight;
                        if (debug && exit_photon.number == debugPhoton)
                            cerr << "caught by R sphere \n" << exit_photon << endl;
                        exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
                        exit_photon.coordinate -= stepLightTunnel * exit_photon.direction;
                        exit_photon.coordinate -= stepSphere * exit_photon.direction;
                    } else {
                        T stepLightTunnel2 = abs(0.009 / exit_photon.direction.z);
                        exit_photon.coordinate += stepLightTunnel2 * exit_photon.direction;
                        if (debug && exit_photon.number == debugPhoton)
                            cerr << "in light tunnel2 \n"<< exit_photon << endl;
                        if ((sqr(exit_photon.coordinate.x) + sqr(exit_photon.coordinate.y)) >= sqr(mainSphereT.getDPort2() / 2)) {
                            SpheresArrayT[i].totalLight += 0.3 * exit_photon.weight;
                            if (debug && exit_photon.number == debugPhoton)
                                cerr << "caught by tunnel \n" << exit_photon << endl;
                            exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
                            exit_photon.coordinate -= stepLightTunnel * exit_photon.direction;
                            exit_photon.coordinate -= stepSphere * exit_photon.direction;
                            exit_photon.coordinate -= stepLightTunnel2 * exit_photon.direction;
                        } else {
                            exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
                            exit_photon.coordinate -= stepLightTunnel * exit_photon.direction;
                            exit_photon.coordinate -= stepSphere * exit_photon.direction;
                            exit_photon.coordinate -= stepLightTunnel2 * exit_photon.direction;
                        }
                    }
                } else {
                    SpheresArrayT[i].totalLight += 0.3 * exit_photon.weight;
                    if (debug && exit_photon.number == debugPhoton)
                        cerr << "caught by tunnel \n" << exit_photon << endl;
                    exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
                    exit_photon.coordinate -= stepLightTunnel * exit_photon.direction;
                }
            } else
                exit_photon.coordinate -= stepDarkTunnel * exit_photon.direction;
        } else
            break;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::FirstReflection(Photon<T>& photon) {
    using namespace Math_NS;
    using namespace Physics_NS;
    using namespace Utils_NS;
    using namespace std;

    const auto ni = sample.getNvacUpper();
    const auto nt = sample.getMedium(0).getN();
    const auto cosi = abs(photon.direction.z);
    const auto cost = TransmittanceCos(ni, nt, cosi);

    T Ri = FresnelReflectance<T>(ni, nt, cosi);

    if (sample.getMedium(0).getMut() == 0) { // specular from glass
        const auto R1 = FresnelReflectance<T>(ni, nt, cosi);
        const auto n3 = sample.getMedium(1).getN();
        const auto R2 = FresnelReflectance<T>(nt, n3, cost);
        Ri = R1 + (sqr(1 - R1) * R2) / (1 - R1 * R2);
    }

    const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const size_t ir = floor(r / dr);

    RRspecular(min(ir, Nr-1)) += Ri * photon.weight;

    auto exitCoord = photon.coordinate; // this is for normal incidence only!
    auto exitDir = Vector3D<T>(photon.direction.x, photon.direction.y, -photon.direction.z);
    auto exitWeight = Ri * photon.weight;
    Photon<T> exitPhoton = Photon<T>(exitCoord, exitDir, exitWeight, photon.number);
    PhotonDetectionSphereR(exitPhoton);

    photon.weight *= (1 - Ri);
    photon.direction.z = TransmittanceCos(ni, nt, cosi);
    // photon.coordinate.z += 1E-9; // crook
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::HopDropSpin(Photon<T>& photon) {
    if (sample.getMedium(photon.layer).getMut() == 0)
        HopInGlass(photon);
    else
        if (homogenous) {
            HopDropSpinInTissue(photon);
            Roulette(photon);
        } else
            HopDropSpinInHeterogeneousTissue(photon);
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
void MonteCarlo<T,Nz,Nr,detector>::HopDropSpinInHeterogeneousTissue(Photon<T>& photon) {
    using namespace std;

    ///1. make border array: find border point (jump to border), bresenham through coag, if there is change in properties, add coag border
    ///2. make coeffs array
    ///3. coag borders with reflection routine, add crossOrNot
    ///4. standard DropSpin routine

    vector<Vector3D<T>> bordersArray;
    vector<T> attCoeffs;
//    InnerBordersArray(photon, bordersArray, attCoeffs);
    /// bordersArray size will always be >= 2.
        /// coag borders routine
    if (debug && photon.number == debugPhoton) {
        cout << "Before HeteroHop" << endl;
        cout << photon << endl;
    }
 //   HopInHeterogeneousTissue(photon, bordersArray, attCoeffs);
    HopInHeterogeneousTissueNoBorder(photon);
    if (debug && photon.number == debugPhoton) {
        cout << "After HeteroHop" << endl;
        cout << photon << endl;
    }
/*    Drop(photon);
    if (debug && photon.number == debugPhoton) {
        cout << "After Drop" << endl;
        cout << photon << endl;
    }
    Spin(photon);
    if (debug && photon.number == debugPhoton) {
        cout << "After Spin" << endl;
        cout << photon << endl;
    }*/
}

template < typename T, size_t Nz, size_t Nr, bool detector>
std::vector<Vector3D<int>> MonteCarlo<T,Nz,Nr,detector>::TrajectoryArrayInt(Photon<T>& photon, Vector3D<T>& finalBorderPoint) {
    using namespace std;
    vector<Vector3D<int>> trajectoryArrayInt;
    T step;
    const auto uz = photon.direction.z;
    if (uz > 0)
        step = (sample.CurrentLowerBorderZ(photon.layer) - photon.coordinate.z) / uz;
    else if (uz < 0)
        step = (sample.CurrentUpperBorderZ(photon.layer) - photon.coordinate.z) / uz;

    finalBorderPoint = photon.coordinate + step * photon.direction;
    Vector3D<T> startPoint = photon.coordinate;
    Bresenham3D(CartesianGridPoint(startPoint), CartesianGridPoint(finalBorderPoint), trajectoryArrayInt);
/*    if (debug && photon.number == debugPhoton) {
        for (auto x : trajectoryArrayInt)
            cerr << x << endl;
        cerr << endl;
    }*/
    return trajectoryArrayInt;
}


template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::InnerBordersArray(Photon<T>& photon, std::vector<Vector3D<T>>& bordersArray, std::vector<T>& attCoeffs) {
    using namespace std;
    Vector3D<T> finalBorderPoint;
    Vector3D<T> startPoint = photon.coordinate;
    vector<Vector3D<int>> trajectoryArrayInt = TrajectoryArrayInt(photon, finalBorderPoint);

    Vector3D<int> point = trajectoryArrayInt[0];
    T val = heterogeneousMatrix3D[point.z](point.x, point.y);

    for (int i = 0; i < trajectoryArrayInt.size() - 1; i++) {
//        Vector3D<int> point = trajectoryArrayInt[i];
        Vector3D<int> point2 = trajectoryArrayInt[i + 1];
        T val2 = heterogeneousMatrix3D[point2.z](point2.x, point2.y);
        /// we will set glass value in the matrix to -1
        if (val != val2) {
           bordersArray.push_back(CartesianCoord(Vector3D<int>(point.x, point.y, point.z)));
           attCoeffs.push_back(usFunc(val)+uaFunc(val));
           attCoeffs.push_back(usFunc(val2)+uaFunc(val2));
        }
        point = point2;
        val = val2;

    }
    bordersArray.push_back(finalBorderPoint);
    bordersArray.insert(bordersArray.begin(), startPoint);
    if (bordersArray.size() == 2) {
        Vector3D<int> point = trajectoryArrayInt[0];
        attCoeffs.push_back(usFunc(heterogeneousMatrix3D[point.z](point.x, point.y))+uaFunc(heterogeneousMatrix3D[point.z](point.x, point.y)));
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::HopInHeterogeneousTissue(Photon<T>& photon, const std::vector<Vector3D<T>>& bordersArray, const std::vector<T>& attCoeffs) {
    using namespace std;

    vector<Vector3D<T>> bordersArrayFull = bordersArray;
    vector<T> attCoeffsFull = attCoeffs;
    int currentBand = 0;
    T xi = random<T>(0, 1);
    photon.step = - log(xi) / attCoeffsFull[currentBand];
    while ((photon.coordinate.z + photon.step * photon.direction.z > bordersArrayFull[currentBand + 1].z && photon.direction.z > 0) ||
           (photon.coordinate.z + photon.step * photon.direction.z < bordersArrayFull[currentBand + 1].z && photon.direction.z < 0)) {
        currentBand += 1;
        photon.coordinate = bordersArrayFull[currentBand];
        if (photon.coordinate.x > radius || photon.coordinate.y > radius)
            photon.alive = false;
        if (photon.coordinate == bordersArrayFull[bordersArrayFull.size() - 1]) {
            CrossOrNot(photon);
            T distToBnd = abs((bordersArrayFull[bordersArrayFull.size() - 1].z - bordersArrayFull[bordersArrayFull.size() - 2].z) / photon.direction.z);
            if (photon.direction.z != 0 && photon.step > distToBnd) {
                photon.stepLeft = (photon.step - distToBnd);
            }
            vector<Vector3D<T>> bordersArrayNew;
            vector<T> attCoeffsNew;
            InnerBordersArray(photon, bordersArrayNew, attCoeffsNew);
            bordersArrayNew.erase(bordersArrayNew.begin());
            bordersArrayFull.insert(bordersArrayFull.end(), bordersArrayNew.begin(), bordersArrayNew.end());
            attCoeffsFull.insert(attCoeffsFull.end(), attCoeffsNew.begin(), attCoeffsNew.end());
            if ((photon.coordinate.z + photon.stepLeft * photon.direction.z < bordersArrayFull[currentBand + 1].z && photon.direction.z > 0) ||
                (photon.coordinate.z + photon.stepLeft * photon.direction.z > bordersArrayFull[currentBand + 1].z && photon.direction.z < 0)) {
                photon.step = photon.stepLeft;
                photon.stepLeft = 0;
                break;
            }
        }
        photon.step = -log(xi) / attCoeffsFull[currentBand];
        for (int i = 0; i < currentBand; i++)
            photon.step -= attCoeffsFull[i] * abs((bordersArrayFull[i+1].z - bordersArrayFull[i].z) / photon.direction.z) / attCoeffsFull[currentBand];
    }
    photon.coordinate += photon.step * photon.direction;
    if (photon.coordinate.x > radius || photon.coordinate.y > radius)
        photon.alive = false;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::HopInHeterogeneousTissueNoBorder(Photon<T>& photon) {
    using namespace std;
    if ((debug&& photon.number == debugPhoton))
        cerr << "alive? " << photon.alive << endl;
    while (photon.alive) {
    T xi = random<T>(0, 1);
    T coord0 = -log(xi);
    T coord = 0;
    T stepTotal = 0;
    Vector3D<T> finalBorderPoint;

    int currentPoint = 0;
    int refCount = 0;
    vector<Vector3D<int>> trajectoryArrayInt = TrajectoryArrayInt(photon, finalBorderPoint);
    Vector3D<int> currentCoordInt = trajectoryArrayInt[currentPoint];
    Vector3D<int> prevCoordInt;
    T weight = photon.weight;
    auto startCoord = photon.coordinate;
    auto startDir = photon.direction;
    bool myDebug = 0;

    if (debug && photon.number == debugPhoton)
        cerr << "coord0 " << coord0 << endl;

    if ((debug && photon.number == debugPhoton )|| myDebug) {
        cerr << "Start COORD " << photon.coordinate << endl;
        cerr << "Start DIR " << photon.direction << endl;
        cerr << "weight " << photon.weight << endl;
        for (auto x : trajectoryArrayInt) {
            cerr << CartesianCoord(x) << endl;
            cerr << x << endl;
        }
    }
    if (debug && photon.number == debugPhoton)
        cerr << "current point here" <<currentPoint << endl;
    while(coord < coord0) {
        if (currentPoint == (trajectoryArrayInt.size() - 1)) {
            refCount += 1;
            if ((debug && photon.number == debugPhoton) || myDebug)
                cerr << "REFLECTION HETERO" << endl;
            photon.coordinate = CartesianCoord(currentCoordInt);

            T Mua = uaFunc<T>(heterogeneousMatrix3D[currentCoordInt.z](currentCoordInt.x, currentCoordInt.y));
            T Mus = usFunc<T>(heterogeneousMatrix3D[currentCoordInt.z](currentCoordInt.x, currentCoordInt.y));
            T Mut = Mua + Mus;
            T ds = distance<T>(CartesianCoord(currentCoordInt), CartesianCoord(prevCoordInt));
            stepTotal += ds;
            coord += Mut * ds;

            const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
            const size_t ir = floor(r / dr);
            const size_t iz = currentCoordInt.z;
   //         A(iz, min(ir, Nr-1)) += photon.weight * (-exp(-Mua * ds) + 1);
   //         photon.weight *= exp(-Mua * ds);
            Roulette(photon);

            CrossOrNot(photon);
            if (sample.getMedium(photon.layer).getMut() == 0)
                HopInGlass(photon);
            else {
                trajectoryArrayInt = TrajectoryArrayInt(photon, finalBorderPoint);
                currentPoint = 1;
                prevCoordInt = trajectoryArrayInt[0];
                currentCoordInt = trajectoryArrayInt[1];
                Roulette(photon);
                if (photon.alive == 0)
                    break;
            }
        } else {
            prevCoordInt = currentCoordInt;
            currentPoint += 1;
            if ((debug&& photon.number == debugPhoton)  || myDebug)
                cerr << "current point " <<currentPoint << endl;
            currentCoordInt = trajectoryArrayInt[currentPoint];
            if (currentCoordInt.x > 2 * Nr - 1 || currentCoordInt.y >= 2 * Nr - 1 || currentCoordInt.z >= Nz || currentCoordInt.x <= 0 || currentCoordInt.y <= 0 || currentCoordInt.z < 0) {
                photon.alive = 0;
                break;
            }

    if ((debug&& photon.number == debugPhoton)  || myDebug)
           cerr << "CURRENT COORD " << CartesianCoord(currentCoordInt) << " " << currentCoordInt << endl;
      if ((debug&& photon.number == debugPhoton)  || myDebug)
         cerr << "PREV COORD " << CartesianCoord(prevCoordInt) << " " << prevCoordInt <<  endl;

 //       cerr << uaFunc<T>(heterogeneousMatrix3D[230](8+Nr, 8+Nr)) << " " << usFunc<T>(heterogeneousMatrix3D[230](8+Nr, 8+Nr)) << " " << gFunc<T>(heterogeneousMatrix3D[230](8+Nr, 8+Nr)) << endl;
        T Mua = uaFunc<T>(heterogeneousMatrix3D[prevCoordInt.z](prevCoordInt.x, prevCoordInt.y));
        T Mus = usFunc<T>(heterogeneousMatrix3D[prevCoordInt.z](prevCoordInt.x, prevCoordInt.y));
        T Mut = Mua + Mus;
  //      if (myDebug)
  //          cerr << Mut << endl;
        T ds = distance<T>(CartesianCoord(currentCoordInt), CartesianCoord(prevCoordInt));
        stepTotal += ds;
        coord += Mut * ds;
        if ((debug && photon.number == debugPhoton))
            cerr << coord << endl;
        photon.coordinate = CartesianCoord(currentCoordInt);

        if (photon.coordinate.x > radius || photon.coordinate.y > radius) {
            cerr << photon.coordinate << " i am too big" << endl;
            cerr << startCoord << endl;
            cerr << startDir << endl;
            cerr << photon.direction << endl;
            cerr << refCount << endl;
            cerr << photon.weight << endl;
            for (auto x : trajectoryArrayInt)
                cerr << x.x << " " << x.y << " " << x.z << endl;
            photon.alive = false;
            break;
        }
        const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
        const size_t ir = floor(r / dr);
        const size_t iz = currentCoordInt.z;
 //       A(iz, min(ir, Nr-1)) += photon.weight * (-exp(-Mua * ds) + 1);
 //       photon.weight *= exp(-Mua * ds);
        Roulette(photon);
        if ((debug&& photon.number == debugPhoton)  || myDebug)
            cerr << "weight " << photon.weight << endl;
        if (photon.alive == 0)
            break;
    }
//    if (CartesianGridPoint(photon.coordinate).z == 29 || CartesianGridPoint(photon.coordinate).z == 59)
  //      cerr << CartesianGridPoint(photon.coordinate).z << endl;
    }
    if ((debug&& photon.number == debugPhoton) || myDebug)
        cerr << "FINAL COORDINATE " << photon.coordinate << endl;
    if ((debug&& photon.number == debugPhoton) || myDebug)
        cerr << "FINAL DIRECTION " << photon.direction << endl;
    if ((debug && photon.number == debugPhoton) || myDebug)
        cerr << "weight " << photon.weight << endl;


    const int layer = photon.layer;
    const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const size_t ir = floor(r / dr);
    const size_t iz = currentCoordInt.z;

    if (iz >= Nz) {
        cout << "ACHTUNG!!! iz = " << iz << " exceeds Nz during drop of photon N " << photon.number << endl;
        cerr << photon.coordinate << endl;
        cerr << photon.direction << endl;
    }
    T Mua, Mut, Mus;
    if (homogenous) {
        Mua = sample.getMedium(layer).getMua();
        Mut = sample.getMedium(layer).getMut();
        Mus = sample.getMedium(layer).getMus();
    } else {
        Mua = uaFunc<T>(heterogeneousMatrix3D[currentCoordInt.z](currentCoordInt.x, currentCoordInt.y));
        Mus = usFunc<T>(heterogeneousMatrix3D[currentCoordInt.z](currentCoordInt.x, currentCoordInt.y));
        Mut = Mua + Mus;
    }
    A(iz, min(ir, Nr-1)) += photon.weight * Mua / Mut;

    photon.weight *= Mus / Mut;
    Spin(photon);
    Roulette(photon);
    }
}
/*
template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::HopInHeterogeneousTissueNoBorder(Photon<T>& photon) {
    T xi = random<T>(0, 1);
    T sleft = -log(xi);
    while (sleft > 0) {
        auto currentCoordInt = CartesianGridPoint<T>(photon.coordinate);
        T Mus = usFunc<T>(heterogeneousMatrix3D[currentCoordInt.z](currentCoordInt.x, currentCoordInt.y));
        T s = sleft / Mus;
        auto tempCoord = photon.coordinate + photon.direction * s;
}*/

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
    using namespace Math_NS;
    using namespace std;

    T mT;
    if (homogenous)
        mT = sample.getMedium(photon.layer).getMut();
    else {
        Vector3D<int> point = CartesianGridPoint(photon.coordinate);
        T Mua = uaFunc<T>(heterogeneousMatrix3D[point.z](point.x, point.y));
        T Mus = usFunc<T>(heterogeneousMatrix3D[point.z](point.x, point.y));
        mT = Mua + Mus;
    }
    if (photon.stepLeft == 0) // new step
        photon.step = -log(random<T>(0, 1)) / mT;
    else { // leftover step
        photon.step = photon.stepLeft / mT;
        photon.stepLeft = 0;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::RecordR(Photon<T>& photon, const T& FRefl, const T& cosT) {
    using namespace Math_NS;
    using namespace Utils_NS;
    using namespace std;

    const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const size_t ir = floor(r/dr);

    RR(min(ir, Nr-1)) += (1 - FRefl) * photon.weight;

    if (debug && photon.number == debugPhoton) {
        cout << "RR at " << ir << " = " << RR(min(ir, Nr-1)) << endl;
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
    using namespace Math_NS;
    using namespace Utils_NS;
    using namespace std;

    const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const size_t ir = floor(r/dr);

    TT(min(ir, Nr-1)) += (1 - FRefl) * photon.weight;

    if (debug && photon.number == debugPhoton) {
        cout << "TT at " << ir << " = " << TT(min(ir, Nr-1)) << endl;
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
    using namespace Math_NS;
    using namespace Utils_NS;
    using namespace std;

    const int layer = photon.layer;
    const auto r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const size_t ir = floor(r / dr);
    const size_t iz = abs(floor(photon.coordinate.z / dz));


    if (iz >= Nz) {
        cout << "ACHTUNG!!! iz = " << iz << " exceeds Nz during drop of photon N " << photon.number << endl;
        cerr << photon.coordinate << endl;
        cerr << photon.direction << endl;
    }
    T Mua, Mut, Mus;
    Vector3D<int> point = CartesianGridPoint(photon.coordinate);
    if (homogenous) {
        Mua = sample.getMedium(layer).getMua();
        Mut = sample.getMedium(layer).getMut();
        Mus = sample.getMedium(layer).getMus();
    } else {
        Mua = uaFunc<T>(heterogeneousMatrix3D[point.z](point.x, point.y));
        Mus = usFunc<T>(heterogeneousMatrix3D[point.z](point.x, point.y));
        Mut = Mua + Mus;
    }
    A(point.z, min(ir, Nr-1)) += photon.weight * Mua / Mut;

    photon.weight *= Mus / Mut;
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::Spin(Photon<T>& photon) {
    using namespace Math_NS;
    using namespace Utils_NS;
    using namespace std;

    const int layer = photon.layer;
    T g;
    if (homogenous)
        g = sample.getMedium(layer).getG();
    else {
        Vector3D<int> point = CartesianGridPoint(photon.coordinate);
        g = gFunc<T>(heterogeneousMatrix3D[point.z](point.x, point.y));
    }

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
    const auto temp  = sqrt(1 - sqr(uz));

    T uxx = +sinHG * (ux * uz * cos(phi) - uy * sin(phi)) / temp + ux * cosHG;
    T uyy = +sinHG * (uy * uz * cos(phi) + ux * sin(phi)) / temp + uy * cosHG;
    T uzz = -sinHG *            cos(phi)                  * temp + uz * cosHG;

    if (abs(abs(uz) - 1)  < 1E-6) {
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
        photon.stepLeft = (photon.step - distToBnd) * sample.getMedium(photon.layer).getMut();
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
    using namespace Math_NS;
    using namespace Physics_NS;
    using namespace std;

    if (debug && photon.number == debugPhoton)
        cout << "CrossUpOrNot" << endl;

    const T cosi = photon.direction.z;
    const int layer = photon.layer;
    const auto ni = sample.getMedium(layer).getN();
    const auto nt = layer == 0 ? sample.getNvacUpper() : sample.getMedium(layer - 1).getN();
    const auto cost = TransmittanceCos(ni, nt, cosi);
    const auto Ri = FresnelReflectance<T>(ni, nt, cosi);
    const auto RND = random<T>(0, 1); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton)
        cout << "cost = " << cost << " FresnelReflectance = " << Ri << " RND = " << RND << endl;

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
    using namespace Math_NS;
    using namespace Physics_NS;
    using namespace std;

    if (debug && photon.number == debugPhoton)
        cout << "CrossDownOrNot" << endl;
    const auto cosi = photon.direction.z;
    const int layer = photon.layer;
    const auto ni = sample.getMedium(layer).getN();
    const auto nt = layer == sample.getNlayers()-1 ? sample.getNvacLower() : sample.getMedium(layer + 1).getN();
    const auto cost = TransmittanceCos(ni, nt, cosi);
    const auto Ri = FresnelReflectance<T>(ni, nt, cosi);
    const auto RND = random<T>(0, 1); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton)
        cout << "cost = " << cost << " FresnelReflectance = "<< Ri << " RND = "<< RND << endl;

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
    using namespace Math_NS;
    using namespace std;

    if (photon.weight < threshold) {
        const auto RND = random<T>(0, 1);
        if (debug && photon.number == debugPhoton)
            cout << "Kill? RND = " << RND << endl;
        if (RND <= chance)
            photon.weight /= chance;
        else{
            if (debug && photon.number == debugPhoton)
                cout << "Yes, kill" << endl;
            photon.alive = false;
        }
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T,Nz,Nr,detector>::Simulation(Photon<T>& photon, const int& num) {
    using namespace Math_NS;
    using namespace std;

    const auto startCoord = lightSource.getPhotonCoord();
    results.sourceMatrix.push_back({startCoord.x, startCoord.y});

    const auto startDir = Vector3D<T>(0, 0, 1); // normal incidence for now
    photon = Photon<T>(startCoord, startDir, 1.0, num);
    FirstReflection(photon);
    if (sample.getMedium(0).getMut() == 0) { // 1st layer is glass -- go directly to tissue
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
    return 2 * M_PI * (ir - 0.5) * Math_NS::sqr(dr);
}

template < typename T, size_t Nz, size_t Nr, bool detector>
Vector3D<int> MonteCarlo<T,Nz,Nr,detector>::CartesianGridPoint(const Vector3D<T>& point) {
//    std::cerr << point.x << " " << point.y << " " << point.z << std::endl;
    int iz = floor(point.z / dz);
    if (point.z < 0)
        iz = 0;
    int ix, iy;

    if (point.x <= -radius)
        ix = 0;
    else if (point.x >= radius)
        ix = 2 * Nr - 3;
    else if (point.x >= 0 && point.x < radius)
        ix = Nr - 1 + floor(point.x / dx);
    else /*(point.x < 0 && point.x > -radius)*/
        ix = Nr - 1 + floor(point.x / dx);

    if (point.y <= -radius)
        iy = 0;
    else if (point.y >= radius)
        iy = 2 * Nr - 3;
    else if (point.y >= 0 && point.y < radius)
        iy = Nr - 1 + floor(point.y / dy);
    else/* (point.y < 0 && point.y > -radius)*/
        iy = Nr - 1 + floor(point.y / dy);

    if (iz == Nz)
        iz -= 1;
    if (iz > Nz)
        std::cerr << "ACHTUNG!!! iz = " << iz << " exceeds Nz while getting cartesian coordinate" << std::endl;
 //   std::cerr << ix << " " << iy << " " << iz << std::endl;
 //   std::cerr << Nz << " " << 2 * Nr - 1 << std::endl;
    return Vector3D<int>(ix, iy, iz);
}

template < typename T, size_t Nz, size_t Nr, bool detector>
Vector3D<T> MonteCarlo<T,Nz,Nr,detector>::CartesianCoord(const Vector3D<int>& point) {
    T x, y, z;
    z = point.z * dz;
    x = (point.x - int(Nr - 1)) * dx;
    y = (point.y - int(Nr - 1)) * dy;

    return Vector3D<T>(x, y, z);
}

template < typename T, size_t Nz, size_t Nr, bool detector>
void MonteCarlo<T, Nz, Nr, detector >::Calculate(MCresults<T,Nz,Nr,detector>& res) {
    using namespace Physics_NS;
    using namespace Utils_NS;
    using namespace std;

    for (int i = 0; i < Nphotons; i++) {
        if (i % 1000 == 0 && homogenous == 0)
            cerr << i << endl;
        Photon<T> myPhoton;
        Simulation(myPhoton, i);
        // cout << RRspecular(0) << endl;
    }

    results.arrayR = RR;
    results.arrayRspecular = RRspecular;
    results.arrayT = TT;
    results.matrixA = A;
    results.heatSource = A;


    for (int i = 0; i < Nz; i++)
        for (int j = 0; j < Nr; j++) {
            results.heatSource(i,j) /= (Volume(j+1));
        }
    results.heatSourceNorm = results.heatSource / Nphotons;
    results.diffuseReflection = RR.sum() / Nphotons;
    results.specularReflection = RRspecular.sum() / Nphotons;
    results.diffuseTransmission = TT.sum() / Nphotons;
    results.absorbed = A.sum() / Nphotons;
    results.arrayAnglesR = arrayAnglesR;
    results.arrayAnglesT = arrayAnglesT;

    results.mainSphereR = mainSphereR;
    results.mainSphereT = mainSphereT;
    results.lightSource = lightSource;

    if (sample.getNlayers() == 1)
        results.BugerTransmission = BugerLambert(sample.getMedium(0).getTau(), sample.getMedium(0).getN(), sample.getNvacLower(), sample.getNvacLower());
    else
        results.BugerTransmission = BugerLambert(sample.getMedium(1).getTau(), sample.getMedium(1).getN(), sample.getMedium(0).getN(), sample.getMedium(2).getN());

    if (detector == 1) {
        results.SpheresArrayR = SpheresArrayR;
        results.SpheresArrayT = SpheresArrayT;
        for (int i = 0; i < isize(SpheresArrayR); i++) {
            /// TODO: use {} instead to make pair
            results.detectedR.push_back({SpheresArrayR[i].getDistance(), SpheresArrayR[i].totalLight / Nphotons});
            results.detectedT.push_back({SpheresArrayT[i].getDistance(), SpheresArrayT[i].totalLight / Nphotons});
        }
    }
    /*
    else {
        for (int i = 0; i < isize(FibersArrayR); i++) {
            results.detectedR.push_back({FibersArrayR[i].getDistance(), FibersArrayR[i].totalLight / Nphotons});
            results.detectedT.push_back({FibersArrayT[i].getDistance(), FibersArrayT[i].totalLight / Nphotons});
        }
    }
    //*/

    res = results;

    /*

    for (int j = 0; j < Nr; j++) {
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
