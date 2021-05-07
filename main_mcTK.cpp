// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MC_TK/Photon.h"
#include "MC_TK/Medium.h"
#include "MC_TK/Fresnel.h"
#include "MC_TK/MonteCarlo.h"
#include "MC_TK/BugerLambert.h"
#include "MC_TK/Sample.h"
#include "MC_TK/MCmultithread.h"

int main (int argc, char **argv) {
    using T = double;

    const int Nz = 1000;
    const int Nr = 10000;

    T selectedRadius = 10e-2;
    T tissueThickness = 4e-3;

    Medium<T> tissue(1.5, 200, 800, tissueThickness, 0.5);

    Medium<T> glass(1.6, 0, 0, 1e-3, 0);

    std::vector<Medium<T>> layers = {glass,tissue,glass};
    Sample<T> mySample(layers, 1.0, 1.0);


    MonteCarlo<T, Nz, Nr> mc(mySample, 1e5, mySample.getTotalThickness(), selectedRadius);

  ////  T reflection, transmission, absorbed;
  ////  MCresults<T,Nz,Nr, > myRes;
  ////  std::thread th1(&MonteCarlo<T, Nz, Nr, >::PhotonsBunchSimulation, std::ref(mc), 0, 2500);

    mc.Calculate();
//fraction
 /*   std::cout << "Diffuse reflection = " << myRes.diffuseReflection << std::endl;
    std::cout << "Specular reflection = " << myRes.specularReflection << std::endl;
    std::cout << "Diffuse transmission = " << myRes.diffuseTransmission << std::endl;
    std::cout << "Absorbed  = " << myRes.absorbed << std::endl;*/

    mc.printResults();
  //  std::cout << "Collimated transmission = " << BugerLambert(tissue.getTau(), tissue.getN(), T(1.0), T(1.0)) << std::endl;

 //   MCmultithread<T,Nz,Nr>(mySample, 1e5, 4, mySample.getTotalThickness(), selectedRadius);

    return 0;
}
