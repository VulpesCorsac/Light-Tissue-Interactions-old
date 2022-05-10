#pragma once

#include <vector>

using namespace Eigen;
using namespace Math_NS;

template < typename T, size_t Nz, size_t Nr>
Matrix<T, Dynamic, Dynamic> coag() {
    Matrix<T,Dynamic,Dynamic> coagMatrix = Matrix<T, Nz, Nr>::Zero();
    for (int i = 0; i < Nz; i++) {
        for (int j = 0; j < Nr; j++) {
            if (i*i + j*j > Math_NS::sqr(Nz/2))
         //   if (i > Nz / 2)
                coagMatrix(i,j) = 1;
        }
    }
    return coagMatrix;
}

