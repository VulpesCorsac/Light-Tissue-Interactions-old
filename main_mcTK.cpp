// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MC_TK/Photon.h"
#include "MC_TK/Medium.h"
#include "MC_TK/MonteCarlo.h"
#include "MC_TK/BugerLambert.h"

int main (int argc, char **argv) {
    using T = float;

    const int Nz = 1000;
    const int Nr = 10000;

    T selectedRadius = 1e-2;
    T tissueThickness = 12e-3;

    Medium<T> tissue(1.5, 50, 950, tissueThickness, 0.0);
    MonteCarlo<T, Nz, Nr> mc(tissue, 1e5, 1, (tissueThickness / Nz), (selectedRadius / Nr), 0.1, 1e-4);

    T reflection, transmission;

    mc.Normalize(reflection, transmission);

    std::cout << "R = " << reflection << std::endl;
    std::cout << "T = " << transmission << std::endl;

    std::cout << "Tuns = " << BugerLambert(tissue.getTau(), tissue.getN(), tissue.getN(), tissue.getN()) << std::endl;

    return 0;
}
