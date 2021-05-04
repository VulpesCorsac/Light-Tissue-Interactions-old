// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MC_TK/Photon.h"
#include "MC_TK/Medium.h"
#include "MC_TK/Fresnel.h"
#include "MC_TK/MonteCarlo.h"
#include "MC_TK/BugerLambert.h"

int main (int argc, char **argv) {
    using T = double;

    const int Nz = 1000;
    const int Nr = 10000;

    T selectedRadius = 10e-2;
    T tissueThickness = 1e-3;

    Medium<T> tissue(1.5, 100, 900, tissueThickness, 0.9);
    MonteCarlo<T, Nz, Nr> mc(tissue, 1e5, 1, (tissueThickness / Nz), (selectedRadius / Nr), 0.1, 1e-4);

    T reflection, transmission, absorbed;
    MCresults<T,Nz,Nr> myRes;
  //  std::thread th1(&MonteCarlo<T, Nz, Nr>::PhotonsBunchSimulation, std::ref(mc), 0, 2500);

    mc.Calculate();

 /*   std::cout << "Diffuse reflection = " << myRes.diffuseReflection << std::endl;
    std::cout << "Specular reflection = " << myRes.specularReflection << std::endl;
    std::cout << "Diffuse transmission = " << myRes.diffuseTransmission << std::endl;
    std::cout << "Absorbed fraction = " << myRes.absorbed << std::endl;*/

    mc.printResults();
    std::cout << "Collimated transmission = " << BugerLambert(tissue.getTau(), tissue.getN(), T(1.0), T(1.0)) << std::endl;
    return 0;
}
