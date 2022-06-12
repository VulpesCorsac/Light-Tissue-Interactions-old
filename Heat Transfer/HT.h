#pragma once

#include <vector>

using namespace Eigen;
using namespace Math_NS;

template < typename T, size_t Nz, size_t Nr>
Matrix<T, Dynamic, Dynamic> coag() {
    Matrix<T,Dynamic,Dynamic> coagMatrix = Matrix<T, Nz, Nr>::Zero();
    for (int i = 0; i < Nz; i++) {
        for (int j = 0; j < Nr; j++) {
            if (sqr(i-Nz/2) + j*j > Math_NS::sqr(Nz/2))
                coagMatrix(i,j) = 1;
            else if (sqr(i-Nz/2) + j*j <= Math_NS::sqr(Nz/2) && sqr(i-Nz/2) + j*j > Math_NS::sqr(Nz/2.5))
                coagMatrix(i,j) = 0.85;
            else if (sqr(i-Nz/2) + j*j <= Math_NS::sqr(Nz/2.5) && sqr(i -Nz/2) + j*j > Math_NS::sqr(Nz/3))
                coagMatrix(i,j) = 0.7;
            else if (sqr(i-Nz/2) + j*j <= Math_NS::sqr(Nz/3) && sqr(i-Nz/2) + j*j > Math_NS::sqr(Nz/3.5))
                coagMatrix(i,j) = 0.55;
            else if (sqr(i-Nz/2) + j*j <= Math_NS::sqr(Nz/3.5) && sqr(i-Nz/2) + j*j > Math_NS::sqr(Nz/4))
                coagMatrix(i,j) = 0.4;
            else if (sqr(i-Nz/2) + j*j <= Math_NS::sqr(Nz/4) && sqr(i-Nz/2) + j*j > Math_NS::sqr(Nz/4.5))
                coagMatrix(i,j) = 0.25;
            else if (sqr(i-Nz/2) + j*j <= Math_NS::sqr(Nz/4.5) && sqr(i-Nz/2) + j*j > Math_NS::sqr(Nz/5))
                coagMatrix(i,j) = 0.1;
               // cerr << i << " " << j << " " << sqr(i) * (-T(i)/T(Nz) + 1.5) / sqr(Nz) + sqr(j) * (-T(j)/T(Nz) + 1.5) / sqr(Nz) << endl;;
              //  coagMatrix(i,j) = Math_NS::sqr(i) * (-T(i)/T(Nz/2) + 1.5) / Math_NS::sqr(Nz/2) + Math_NS::sqr(j) * (-T(j)/T(Nz/2) + 1.5) / Math_NS::sqr(Nz/2);
        }
    }
    return coagMatrix;
}

