// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MC_TK/Photon.h"
#include "MC_TK/Medium.h"
#include "MC_TK/Fresnel.h"
#include "MC_TK/MonteCarlo.h"
#include "MC_TK/BugerLambert.h"
#include "MC_TK/Sample.h"
#include "MC_TK/MCmultithread.h"
#include "Tests/TestMC.h"

int main (int argc, char **argv) {
    using T = double;

    const int Nz = 1000;
    const int Nr = 10000;

    T selectedRadius = 10e-2;

    Medium<T> tissue(1.5, 1000, 0, 1e-3, 0.9);

    Medium<T> glass1(1.6, 0, 0, 1e-3, 0);
    Medium<T> glass2(1.65, 0, 0, 1e-3, 0);

    std::vector<Medium<T>> layers = {glass1,tissue,glass1};
    Sample<T> mySample(layers, 1.0, 1.0);

  /*  MonteCarlo<T, Nz, Nr> mc(mySample, 1e5, mySample.getTotalThickness(), selectedRadius);
    MCresults<T,Nz,Nr> myResults;
    mc.Calculate(myResults);
    printResults(myResults);*/

 /*   MCresults<T,Nz,Nr> myResultsMT;
    MCmultithread<T,Nz,Nr>(mySample, 10e6, 4, mySample.getTotalThickness(), selectedRadius, myResultsMT);
    printResults(myResultsMT);
    std::cout << "Collimated transmission = " << BugerLambert(tissue.tau, tissue.n, T(1.6), T(1.6)) << std::endl;
*/
    TestsMC test;
    test.SingleLayerAbsorptionOnly();
    test.MultiLayerAbsorptionOnly();
    test.SingleLayerAbsorptionScattering();
    test.MultiLayerAbsorptionScattering();

    return 0;
}
