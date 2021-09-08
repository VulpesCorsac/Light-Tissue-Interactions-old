// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MC/Medium.h"
#include "MC/BugerLambert.h"
#include "MC/Photon.h"
#include "MC/Fresnel.h"
#include "MC/Sample.h"
#include "MC/Detector.h"
#include "MC/MonteCarlo.h"
#include "MC/MCmultithread.h"
#include "MC/IMC.h"
//#include "Tests/TestMCStandalone.h"
//#include "Tests/Thresholds.h"

int main(int argc, char **argv) {
    using namespace std;
    using T = double;

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr int Nphotons = 1e6;
    constexpr T selectedRadius = 10e-2;

    // auto tissue = Medium<T>::fromCoeffs(1.5, 100, 900, 1e-3, 0.9);
    auto tissue = Medium<T>::fromAlbedo(1.5, 0.9, 1.0, 1e-3, 0.9);

    // Medium<T> tissue(1.5, 100, 900, 1e-3, 0.9);
    auto glass1 = Medium<T>::fromCoeffs(1.5, 0.0, 0.0, 1e-3, 0.0);
    auto glass2 = Medium<T>::fromCoeffs(1.65,    0, 0, 1e-3,   0);

    vector<Medium<T>> layers = {tissue};
    Sample<T> mySample(layers, 1.0, 1.0);

    const bool detector = 1; // spheres => detector = 1; fiber => detector = 0.
    IntegratingSphere<T> SphereT(0.1, 0.013, 0.013); // dPort2 = zero if the sphere has one port
    IntegratingSphere<T> SphereR(0.1, 0.013, 0.013);

    DetectorDistance<T> distances;
    distances.max = 0.2;
    distances.min = 0.0;
    distances.step = 0.02; // please, enter correct step for your borders

    /*
    MonteCarlo<T, Nz, Nr, detector> mc(mySample, Nphotons, mySample.getTotalThickness(), selectedRadius, SphereR, SphereT, distances);
    MCresults<T,Nz,Nr,detector> myResults;
    mc.Calculate(myResults);
    cout << myResults << endl;
    //*/

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(mySample, Nphotons, 4, mySample.getTotalThickness(), selectedRadius, myResultsMT, SphereR, SphereT, distances);
    cout << myResultsMT << endl;

    auto rsmeas = myResultsMT.detectedR;
    auto tsmeas = myResultsMT.detectedT;
    T tcmeas = 0.34;

    auto emptyTissue = Medium<T>::fromAlbedo(1.5, 0.0, 0.0, 1e-3, 0.0);
    vector<Medium<T>> slides = {};

    T aOut, gOut, tauOut;

    T aStart = 0.833451;
    T gStart = 0.904972;
    T tStart = 0.997383;

    T checkConvEps = 1e-4;

    IMC<T,Nz,Nr,detector,N,fix>(rsmeas, tsmeas, tcmeas, emptyTissue, std::move(slides), Nphotons, 4, emptyTissue.D, selectedRadius, SphereR, SphereT, distances, aStart, tStart, gStart, checkConvEps, aOut, tauOut, gOut);

    cout << aOut << " " << tauOut << " " << gOut << endl;

    /*
    evaluateThresholds();
    // return 0;

    TestsMC test;
    constexpr int runs = 1;
    for (int i = 0; i < runs; ++i) {
        cout << "SingleLayerAbsorptionOnly: " << i+1 << endl;
        test.SingleLayerAbsorptionOnly();
    }
    for (int i = 0; i < runs; ++i) {
        cout << "MultiLayerAbsorptionOnly: " << i+1 << endl;
        test.MultiLayerAbsorptionOnly();
    }
    for (int i = 0; i < runs; ++i) {
        cout << "SingleLayerAbsorptionScattering: " << i+1 << endl;
        test.SingleLayerAbsorptionScattering();
    }
    for (int i = 0; i < runs; ++i) {
        cout << "MultiLayerAbsorptionScattering: " << i+1 << endl;
        test.MultiLayerAbsorptionScattering();
    }
    //*/

    return 0;
}
