#include "../Scripts/DirectHeterogeneousMC.h"
#include "../teploprovod/heat_transfer.h"

#include "../eigen/Eigen/Dense"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    using namespace Inverse_NS;
    using namespace std;
    using namespace Eigen;
    using T = double;

    constexpr int M = 8; // matrix size in Adding-Doubling
    constexpr int Nz = 98;
    constexpr int Nr = 501;
    constexpr bool detector = 1; // spheres => detector = 1; fiber => detector = 0.
    Matrix<T, Dynamic, Dynamic> coagMatrix = Matrix<T, Nz, Nr>::Zero();

    ofstream H;
    H.open("HeatMatrix.xls");
    T time = 0, t_end = 10;

    while (time < t_end) {
      MCresults<T,Nz,Nr,detector> myRes = directHeterogeneousMC<T,Nz,Nr,detector>(0.83, 0.6, 0.0, 1.5, 0.0, 1.4, 0.001, 1.5, 0.0, coagMatrix, 0, 1, 0);
      coagMatrix = HeatTransfer<T,Nz,Nr,detector>(20, 1, 50, 50, 20, time, 0.1, 0.1, 0.01, myRes.heatSource); 
      time += 1;
    }


    return 0;
}
