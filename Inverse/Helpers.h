#pragma once

#include "FixedParam.h"

#include "../Math/Basic.h"

#include "../eigen/Eigen/Dense"

#include "../AD/Eigen_NS.h"

namespace Inverse_NS {
    template < typename T, size_t N, size_t gSize, Inverse_NS::FixedParameter fix >
    void ConstructGrid(Matrix<T,1,gSize>& gridA, Matrix<T,1,gSize>& gridT, Matrix<T,1,gSize>& gridG) {
        using namespace Math_NS;
        using namespace std;

        T tMin = -5;
        T tMax = 3;
/*        if ((fix == FixedParameter::G && N == 2) || N == 3) {
            cout << "Thick (tau > 2) or thin (tau < 2)? Thick = 0, thin = 1" << endl;
            int thick;
            cin >> thick;
            if (thick == 0) {
                tMin = 0;
                tMax = 4;
            } else if (thick == 1) {
                tMin = -5;
                tMax = 0;
            }
        }*/

        for (size_t i = 0; i < gSize; i++) {
            const T j = i;
            const T x = j / (gSize - 1.0);
            if (x < 0.25)
                gridA(i) = 0.9999 * (1 - x) + 0.00001;
            else if (x > 0.75)
                gridA(i) = 0.9999 * (1 - x) + 0.00001;
            else
                gridA(i) = 0.9999 * sqr(1 - x) + 0.00001;
            gridT(i) = pow(2, tMin + (tMax - tMin) * x);
            // gridG(i) = 0.9999 * (2.0 * i / (gSize - 1.0) - 1.0) + 0.00001;
            gridG(i) = 0.9999 * i / (gSize - 1.0) + 0.00001;
            // cout << gridA(i) << " " << gridG(i) << endl;
            /*
            if (x < 0.25)
                gridG(i) = 0.9999*(1 - x)+0.00001;
            else if (x > 0.75)
                gridG(i) = 0.9999*(1 - x)+0.00001;
            else
                gridG(i) = 0.9999*sqr(1 - x)+0.00001;
            //*/
        }
    }
}
