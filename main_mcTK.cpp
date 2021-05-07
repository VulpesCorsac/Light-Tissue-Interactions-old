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

    Medium<T> tissue(1.6, 700, 300, 10e-3, 0.0);

    Medium<T> glass1(1.6, 0, 0, 1e-3, 0);
    Medium<T> glass2(1.65, 0, 0, 1e-3, 0);

    std::vector<Medium<T>> layers = {glass2,tissue,glass2};
    Sample<T> mySample(layers, 1.0, 1.0);

  /*  MonteCarlo<T, Nz, Nr> mc(mySample, 1e5, mySample.getTotalThickness(), selectedRadius);
    MCresults<T,Nz,Nr> myResults;
    mc.Calculate(myResults);
    printResults(myResults);*/

  //  std::cout << "Collimated transmission = " << BugerLambert(tissue.getTau(), tissue.getN(), T(1.0), T(1.0)) << std::endl;

/*    MCresults<T,Nz,Nr> myResultsMT;
    MCmultithread<T,Nz,Nr>(mySample, 5e6, 4, mySample.getTotalThickness(), selectedRadius, myResultsMT);
    printResults(myResultsMT);*/

    TestsMC test;
 //   test.MultiLayerAbsorptionScattering();

    return 0;
}
