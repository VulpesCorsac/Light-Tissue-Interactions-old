

#include "MC_TK/Photon.h"
#include "MC_TK/Medium.h"
#include "MC_TK/MonteCarlo.h"



int main (int argc, char **argv) {
    using T = float;

    Matrix<T, 1, 3> startCoord, startDir;
    startCoord << 0, 0, 0;
    startDir << 0, 0, 1;

    Photon<T> photon(startCoord, startDir, 1.0, 0.0);
    Medium<T> tissue(1.4, 100, 1000, 1e-2, 0.9);
    MonteCarlo<T,10,100> mc(tissue, 5e4, 1, 1e-3, 1e-3, 0.1, 1e-4);
    mc.Normalize();


    return 0;
}
