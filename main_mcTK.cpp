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

    Medium<T> tissue(1.5, 10, 990, 4e-3, 0.96);
    MonteCarlo<T, Nz, Nr> mc(tissue, 1e5, 1, (4e-3 / Nz), (selectedRadius / Nr), 0.1, 1e-4);

    T reflection, transmission;

    mc.Normalize(reflection, transmission);

    std::cout << "R = " << reflection << std::endl;
    std::cout << "T = " << transmission << std::endl;

    std::cout << "Tuns = " << BugerLambert(tissue.getTau(), tissue.getN(), tissue.getN(), tissue.getN()) << std::endl;

    return 0;
}
