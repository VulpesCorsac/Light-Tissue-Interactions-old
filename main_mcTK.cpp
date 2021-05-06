// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MC_TK/Photon.h"
#include "MC_TK/Medium.h"
#include "MC_TK/Fresnel.h"
#include "MC_TK/MonteCarlo.h"
#include "MC_TK/BugerLambert.h"
#include "MC_TK/Sample.h"

int main (int argc, char **argv) {
    using T = double;

    const int Nz = 1000;
    const int Nr = 10000;

    const int nLayers = 3;

    T selectedRadius = 10e-2;
    T tissueThickness = 12e-3;

    Medium<T> tissue(1.4, 700, 300, tissueThickness, 0.5);

    Medium<T> glass(1.5, 0, 0, 1e-3, 0);

    std::array<Medium<T>, nLayers> layers = {glass,tissue,glass};
    Sample<T, nLayers> mySample(layers, 1.0, 1.0);


    MonteCarlo<T, Nz, Nr, nLayers> mc(mySample, 1e6, 1, (mySample.getTotalThickness() / Nz), (selectedRadius / Nr), 0.1, 1e-4);

  ////  T reflection, transmission, absorbed;
  ////  MCresults<T,Nz,Nr, nLayers> myRes;
  ////  std::thread th1(&MonteCarlo<T, Nz, Nr, nLayers>::PhotonsBunchSimulation, std::ref(mc), 0, 2500);

    mc.Calculate();

 /*   std::cout << "Diffuse reflection = " << myRes.diffuseReflection << std::endl;
    std::cout << "Specular reflection = " << myRes.specularReflection << std::endl;
    std::cout << "Diffuse transmission = " << myRes.diffuseTransmission << std::endl;
    std::cout << "Absorbed fraction = " << myRes.absorbed << std::endl;*/

    mc.printResults();
 //   std::cout << "Collimated transmission = " << BugerLambert(tissue.getTau(), tissue.getN(), T(1.0), T(1.0)) << std::endl;
    return 0;
}
