// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MC/Photon.h"
#include "MC/Medium.h"
#include "MC/Fresnel.h"
#include "MC/MonteCarlo.h"
#include "MC/BugerLambert.h"
#include "MC/Sample.h"
#include "MC/Detector.h"
#include "MC/MCmultithread.h"
#include "Tests/TestMCStandalone.h"

#include "Tests/Thresholds.h"

int main(int argc, char **argv) {
    using namespace std;
    using T = double;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;

    constexpr int Nphotons = 1e5;

    constexpr T selectedRadius = 10e-2;

    Medium<T> tissue(1.5 , 100, 900, 1e-3, 0.9);
    Medium<T> glass1(1.6 ,    0, 0, 1e-3,   0);
    Medium<T> glass2(1.65,    0, 0, 1e-3,   0);

    vector<Medium<T>> layers = {tissue};
    Sample<T> mySample(layers, 1.0, 1.0);
    //

    MCresults<T,Nz,Nr> myResultsMT;
    MCmultithread<T,Nz,Nr>(mySample, Nphotons, 4, mySample.getTotalThickness(), selectedRadius, myResultsMT);
    cout << myResultsMT << endl;

  /*  MonteCarlo<T, Nz, Nr> mc(mySample, Nphotons, mySample.getTotalThickness(), selectedRadius);
    MCresults<T,Nz,Nr> myResults;
    mc.Calculate(myResults);
    cout << myResults << endl;*/

    //FRONT/RECLECTION IS WHERE THE LASER HITS THE SAMPLE -> ALWAYS 2 PORTS

    SphereTwoPorts<T> SphereR(0.1, 0.01, 0.01, 0.001);
    SphereOnePort<T> SphereT(0.1, 0.01, 0.001);

 /*   T collectedR = SphereR.dataTwoPorts(myResults.exitedPhotonsFront, Nphotons);
    T collectedT = SphereT.dataOnePort(myResults.exitedPhotonsRear, Nphotons);*/
    T collectedRmt = SphereR.dataTwoPorts(myResultsMT.exitedPhotonsFront, Nphotons);
    T collectedTmt = SphereT.dataOnePort(myResultsMT.exitedPhotonsRear, Nphotons);

 //   cout << "Collected R " << collectedR << ", collected T " << collectedT << endl;
    cout << "Collected Rmt " << collectedRmt << ", collected Tmt " << collectedTmt << endl;


    //

    /*
    MCresults<T,Nz,Nr> myResultsMT;
    MCmultithread<T,Nz,Nr>(mySample, 10e6, 4, mySample.getTotalThickness(), selectedRadius, myResultsMT);
    printResults(myResultsMT);
    cout << "Collimated transmission = " << BugerLambert(tissue.tau, tissue.n, T(1.6), T(1.6)) << endl;
    //*/

 /*   evaluateThresholds();
//    return 0;

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
    }//*/

    return 0;
}
