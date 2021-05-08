#pragma once

#include "Photon.h"
#include "Medium.h"
#include "Fresnel.h"
#include "Sample.h"

#include "../Utils/Random.h"

#include "../eigen/Eigen/Dense"

#include <cmath>
#include <tgmath.h>

using namespace Eigen;

template < typename T, size_t Nz, size_t Nr >
struct MCresults {
    T specularReflection = 0;
    T diffuseReflection = 0;
    T diffuseTransmission = 0;
    T absorbed = 0;
    Matrix<T, Dynamic, Dynamic> matrixA = Matrix<T, Nz, Nr>::Constant(0);
    Matrix<T, 1, Dynamic> arrayR = Matrix<T, 1, Nr>::Constant(0);
    Matrix<T, 1, Dynamic> arrayRspecular = Matrix<T, 1, Nr>::Constant(0);
    Matrix<T, 1, Dynamic> arrayT = Matrix<T, 1, Nr>::Constant(0);
};

template < typename T, size_t Nz, size_t Nr >
void printResults(MCresults<T,Nz,Nr>& res) {
    using namespace std;

    cout << "Diffuse reflection = " << res.diffuseReflection << endl;
    cout << "Specular reflection = " << res.specularReflection << endl;
    cout << "Diffuse transmission = " << res.diffuseTransmission << endl;
    cout << "Absorbed fraction = " << res.absorbed << endl;
}

template < typename T, size_t Nz, size_t Nr >
class MonteCarlo {
public:
    MonteCarlo() noexcept = delete;
    MonteCarlo(const Sample<T>& new_sample, const int& new_Np, const T& new_z, const T& new_r);
    ~MonteCarlo() noexcept = default;

    void Calculate(MCresults<T,Nz,Nr>& res);

    inline Matrix<T, Dynamic, Dynamic> getMatrixA() const noexcept { return A; }
    inline Matrix<T, Dynamic, Dynamic> getArrayR() const noexcept { return RR; }
    inline Matrix<T, Dynamic, Dynamic> getArrayRspec() const noexcept { return RRspecular; }
    inline Matrix<T, Dynamic, Dynamic> getArrayT() const noexcept { return TT; }

protected:
    const Sample<T>& sample;

    const int Nphotons;

    const T dz;
    const T dr;
    const T chance;
    const T threshold;

    bool debug = 0;
    int debugPhoton = 0;

    Matrix<T, Dynamic, Dynamic> A = Matrix<T, Nz, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> RR = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> RRspecular = Matrix<T, 1, Nr>::Constant(0.0);
    Matrix<T, 1, Dynamic> TT = Matrix<T, 1, Nr>::Constant(0.0);

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
MonteCarlo<T, Nz, Nr>::MonteCarlo(const Sample<T>& new_sample, const int& new_Np, const T& new_z, const T& new_r)
    : sample(new_sample)
    , Nphotons(new_Np)
    , dz(new_z / Nz)
    , dr(new_r / Nr)
    , chance(0.1)
    , threshold(1e-4) {
}

/*
template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Launch(const Vector3D<T>& startCoord, const Vector3D<T>& startDir, Photon<T>& photon) {
    using namespace std;

    // cout << "LAUNCH" << endl;
    /// TODO: better set these parameters to default and use them explicitly only when needed
    photon = Photon<T>(startCoord, startDir, 1.0, 0.0);
    photon.alive = 1;
}
//*/

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::FirstReflection(Photon<T>& photon) {
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
    const int ir = floor(r/dr);

    RRspecular(ir) += Ri * photon.weight;

    photon.weight *= (1 - Ri);
    photon.direction.z = CosT(ni, nt, cosi);
    // photon.coordinate.z += 1e-9; // crook
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::HopDropSpin(Photon<T>& photon) {
    const int layer = photon.layer;
    /// TODO: unused variable
    const T mT = sample.getMedium(layer).ut;
    if (mT == 0)
        HopInGlass(photon);
    else
        HopDropSpinInTissue(photon);
    Roulette(photon);
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::HopInGlass(Photon<T>& photon) {
    using namespace std;

    if (debug && photon.number == debugPhoton) {
        cout << "Before HopInGlass" << endl;
        photon.printInfo();
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
        photon.printInfo();
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::HopDropSpinInTissue(Photon<T>& photon) {
    using namespace std;

    StepSizeInTissue(photon);
    if (HitBoundary(photon)) {
        if (debug && photon.number == debugPhoton) {
            cout << "Before HopInTissue" << endl;
            photon.printInfo();
        }
        Hop(photon);
        CrossOrNot(photon);
        if (debug && photon.number == debugPhoton) {
            cout << "After HopInTissue" << endl;
            photon.printInfo();
        }
    } else {
        if (debug && photon.number == debugPhoton) {
            cout << "Before Hop" << endl;
            photon.printInfo();
        }
        Hop(photon);
        if (debug && photon.number == debugPhoton) {
            cout << "After Hop" << endl;
            photon.printInfo();
        }
        Drop(photon);
        if (debug && photon.number == debugPhoton) {
            cout << "After Drop" << endl;
            photon.printInfo();
        }
        Spin(photon);
        if (debug && photon.number == debugPhoton) {
            cout << "After Spin" << endl;
            photon.printInfo();
        }
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::StepSizeInGlass(Photon<T>& photon) {
    const int layer = photon.layer;
    const T uz = photon.direction.z;
    T distToBnd = 0;
    if (uz > 0)
        distToBnd = (sample.CurrentLowerBorderZ(layer) - photon.coordinate.z) / uz;
    else if (uz < 0)
        distToBnd = (sample.CurrentUpperBorderZ(layer) - photon.coordinate.z) / uz;
    else
        distToBnd = 0;
    photon.step = distToBnd;
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::StepSizeInTissue(Photon<T>& photon) {
    using namespace std;

    const int layer = photon.layer;
    const T mT = sample.getMedium(layer).ut;
    if (photon.stepLeft == 0) { // new step
        const T RND = random(0.0, 1.0);
        photon.step = -log(RND) / mT;
    } else { // leftover step
        photon.step = photon.stepLeft / mT;
        photon.stepLeft = 0;
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::RecordR(Photon<T>& photon, const T& FRefl) {
    using namespace std;

    const T r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const int ir = floor(r/dr);

    /// TODO: use ternary
    if (ir >= Nr)
        RR(Nr-1) += (1 - FRefl) * photon.weight; // OVERFLOW BIN
    else
        RR(ir) += (1 - FRefl) * photon.weight;

    if (debug && photon.number == debugPhoton) {
        cout << "RR at " << ir << " = " << RR(ir) << endl;
        photon.printInfo();
    }

    photon.weight *= FRefl;
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::RecordT(Photon<T>& photon, const T& FRefl) {
    using namespace std;

    const T r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const int ir = floor(r/dr);

    /// TODO: use ternary
    if (ir >= Nr)
        TT(Nr-1) += (1 - FRefl) * photon.weight; // OVERFLOW BIN
    else
        TT(ir) += (1 - FRefl) * photon.weight;

    if (debug && photon.number == debugPhoton) {
        cout << "TT at " << ir << " = " << TT(ir) << endl;
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
    using namespace std;

    const int layer = photon.layer;
    const T r = sqrt(sqr(photon.coordinate.x) + sqr(photon.coordinate.y));
    const int ir = floor(r / dr);
    int iz = floor(photon.coordinate.z / dz);
    if (iz < 0)
        iz = abs(iz);

    if (iz >= Nz)
        cout << "ACHTUNG!!! iz = " << iz << " exceeds Nz during drop of photon N " << photon.number << endl;

    /// TODO: use ternary
    if (ir >= Nr)
        A(iz, Nr-1) += photon.weight * sample.getMedium(layer).ua / sample.getMedium(layer).ut; // OVERFLOW BIN
    else
        A(iz, ir) += photon.weight * sample.getMedium(layer).ua / sample.getMedium(layer).ut;

    photon.weight *= sample.getMedium(layer).us / sample.getMedium(layer).ut;
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Spin(Photon<T>& photon) {
    using namespace std;

    const int layer = photon.layer;
    const T g = sample.getMedium(layer).g;

    const T RND1 = random(0.0, 1.0);
    T cosHG = (1 + sqr(g) - sqr((1 - sqr(g)) / (1 - g + 2 * g * RND1))) / (2 * g);
    if (g == 0)
        cosHG = 2 * RND1 - 1;
    else if (g == 1)
        cosHG = 1;

    const T RND2 = random(0.0, 1.0);
    const T phi = 2 * M_PI * RND2; // radians

    if (debug && photon.number == debugPhoton)
        cout << "RND1 = " << RND1 << " cosHG = " << cosHG << " RND2 = " << RND2 << " phi = " << phi << endl;

    T ux = photon.direction.x;
    T uy = photon.direction.y;
    T uz = photon.direction.z;

    const T sinHG = sqrt(1 - sqr(cosHG));
    const T temp = sqrt(1 - sqr(uz));

    T uxx = +sinHG * (ux * uz * cos(phi) - uy * sin(phi)) / temp + ux * cosHG;
    T uyy = +sinHG * (uy * uz * cos(phi) + ux * sin(phi)) / temp + uy * cosHG;
    T uzz = -sinHG * cos(phi) * temp                             + uz * cosHG;

    if (abs(uz - 1)  < 1e-6) {
        uxx = sinHG * cos(phi);
        uyy = sinHG * sin(phi);
        /// TODO: ternary operator
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
bool MonteCarlo<T, Nz, Nr>::HitBoundary(Photon<T>& photon) {
    /// TODO: unused variable
    bool hit;
    const int layer = photon.layer;
    const T uz = photon.direction.z;
    const T mT = sample.getMedium(layer).ut;

    T distToBnd = 0;
    if (uz > 0)
        distToBnd = (sample.CurrentLowerBorderZ(layer) - photon.coordinate.z) / uz;
    else if (uz < 0)
        distToBnd = (sample.CurrentUpperBorderZ(layer) - photon.coordinate.z) / uz;

    if (uz != 0 && photon.step > distToBnd) {
        photon.stepLeft = (photon.step - distToBnd) * mT;
        photon.step = distToBnd;
        hit = true;
    } else
        hit = false;
    return hit;

}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::CrossOrNot(Photon<T>& photon) {
    /// TODO: ternary
    if (photon.direction.z < 0)
        CrossUpOrNot(photon);
    else
        CrossDownOrNot(photon);
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::CrossUpOrNot(Photon<T>& photon) {
    using namespace std;

    if (debug && photon.number == debugPhoton)
        cout << "CrossUpOrNot" << endl;
    const T cosi = photon.direction.z;
    const int layer = photon.layer;
    const T ni = sample.getMedium(layer).n;
    /// TODO: ternary
    T nt = 1;
    if (layer == 0)
        nt = sample.getNvacUpper();
    else
        nt = sample.getMedium(layer - 1).n;
    const T cost = CosT(ni, nt, cosi);
    const T Ri = FresnelR(ni, nt, cosi);
    const T RND = random(0.0, 1.0); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton)
        cout << "cost = " << cost << " FresnelR = " << Ri << " RND = " << RND << endl;

    if (layer == 0 && Ri < 1) { // partially transmitted -- only on sample border
        if (debug && photon.number == debugPhoton)
            cout << "SAMPLE BORDER" << endl;
        RecordR(photon, Ri);
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

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::CrossDownOrNot(Photon<T>& photon) {
    using namespace std;

    if (debug && photon.number == debugPhoton)
        cout << "CrossDownOrNot" << endl;
    const T cosi = photon.direction.z;
    const int layer = photon.layer;
    const T ni = sample.getMedium(layer).n;
    /// TODO: ternary
    T nt = 1.0;
    if (layer == sample.getNlayers() - 1)
        nt = sample.getNvacLower();
    else
        nt = sample.getMedium(layer + 1).n;
    const T cost = CosT(ni, nt, cosi);
    const T Ri = FresnelR(ni, nt, cosi);
    const T RND = random(0.0, 1.0); // reflected or transmitted on inner borders?

    if (debug && photon.number == debugPhoton)
        cout << "cost = " << cost << " FresnelR = "<< Ri << " RND = "<< RND << endl;

    if (layer == (sample.getNlayers() - 1) && Ri < 1) { // partially transmitted -- only on sample border
        if (debug && photon.number == debugPhoton)
            cout << "SAMPLE BORDER" << endl;
        RecordT(photon, Ri);
        photon.direction.z = - photon.direction.z;
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
        photon.direction.z = -photon.direction.z;
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Roulette(Photon<T>& photon) {
    using namespace std;

    if (photon.weight < threshold) {
        const T RND = random(0.0, 1.0);
        if (debug && photon.number == debugPhoton)
            cout << "Kill? RND = " << RND << endl;
        if (RND <= chance)
            photon.weight /= chance;
        else
            photon.alive = 0;
    }
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr>::Simulation(Photon<T>& photon, const int& num) {
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

template < typename T, size_t Nz, size_t Nr >
T MonteCarlo<T, Nz, Nr>::Volume(const T& ir) {
    return Area(ir) * dz;
}

template < typename T, size_t Nz, size_t Nr >
T MonteCarlo<T, Nz, Nr>::Area(const T& ir) {
    return 2 * M_PI * (ir - 0.5) * sqr(dr);
}

template < typename T, size_t Nz, size_t Nr >
void MonteCarlo<T, Nz, Nr >::Calculate(MCresults<T,Nz,Nr>& res) {
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
