#include "../Scripts/DirectHeterogeneousMC.h"
#include "../Heat Transfer/HT.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Eigen/Dense>

template < typename T, size_t Nz, size_t Nr >
Matrix<T, Dynamic, Dynamic> readCSV(std::string file) {
    using namespace std;

  ifstream in(file);
  string line;

  int row = 0;
  int col = 0;

  Matrix<T, Dynamic, Dynamic> res = Matrix<T, Nz, Nr>::Zero();

  if (in.is_open()) {
    while (getline(in, line)) {

      char *ptr = (char *) line.c_str();
      int len = line.length();

      col = 0;

      char *start = ptr;
      for (int i = 0; i < len; i++) {

        if (ptr[i] == ',') {
          res(row, col++) = stod(start);
          start = ptr + i + 1;
        }
      }
      res(row, col) = stod(start);

      row++;
    }

    in.close();
  }
  return res;
}


int main() {
    using namespace Inverse_NS;
    using namespace std;
    using namespace Eigen;
    using T = double;

    constexpr int M = 8; // matrix size in Adding-Doubling

    constexpr int Nz = 100;
    constexpr int Nr = 501;
    constexpr bool detector = 1; // spheres => detector = 1; fiber => detector = 0.

    const static IOFormat CSVFormat(FullPrecision, DontAlignCols, ", ", "\n");

    auto coagMatrix = coag<T, Nz, Nr>();
    ofstream coagfile;
    coagfile.open("CoagMatrix.csv");
    if (!coagfile.is_open())
        throw invalid_argument("Failed to open file coag");
    coagfile << coagMatrix.format(CSVFormat) << '\n';
    coagfile.close();

    const string fName = "CoagMatrix.csv";
    auto coagMatrixRead = readCSV<T, Nz, Nr>(fName);
    constexpr int Nthreads = 1;

    MCresults<T,Nz,Nr,detector> myRes = directHeterogeneousMC<T,Nz,Nr,detector>(0.83, 0.6, 0.0, 1.5, 0.00, 1.34, 0.0006, 1.5, 0.00, coagMatrixRead, 0, Nthreads, 0);

    ofstream Afile;
    Afile.open("HeatSourceMatrix.csv");
    if (!Afile.is_open())
        throw invalid_argument("Failed to open file A");
 //   const static IOFormat CSVFormat(FullPrecision, DontAlignCols, ", ", "\n");
    Afile << myRes.heatSource.format(CSVFormat) << '\n';
    Afile.close();

    return 0;
}
