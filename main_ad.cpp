// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "AD/NelderMead.h"
#include "AD/Quadrature.h"
#include "AD/RT.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main (int argc, char **argv) {
    using T = float;
    const int M = 128;

    T a = 0.0; // albedo
    T tau = 1.0; // optical thickness
    T g = 0.9; // anisotropy
    //
    T n_slab = 1.5; // refraction index of sample
    T n_slide_top = 1.6; // refraction index of slide
    T n_slide_bottom = 1.65;

    Quadrature<T,M> quadrature(n_slab);

    const auto v = quadrature.getV();
    const auto w = quadrature.getW();

    /*
    std::cout << "Knots" << std::endl;
    quadrature.printQuadrature(v);
    std::cout << "Weights" << std::endl;
    quadrature.printQuadrature(w);
    //*/

    // const int N = 2; // minimize 2 parameters
    // const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    /*
    std::ofstream myfileIAD;
    myfileIAD.open ("IAD_calc_set.txt");

    // GENERATE SETS OF A, TAU AND G
    Matrix<T, 1, 10> setA, setG, setT;
    setA << 0, 0.1, 0.3, 0.5, 0.7, 0.8, 0.9, 0.95, 0.99, 1.0;
    setG << 0, 0.19, 0.36, 0.51, 0.64, 0.75, 0.84, 0.9, 0.96, 0.99;
    setT << 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 4.0, 6.0, 8.0, 12.0;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 10; k++) {
                T ts, rs, tc = 0;
                tc = Tc<T,M>(setT(j), n_slab, n_slide_top, n_slide_bottom);
                RTs<T,M>(setA(i), setT(j), setG(k), n_slab, n_slide_top, n_slide_bottom, v, w, rs, ts);

                T fixedParam;// g or tau
                if (fix == 1)
                    fixedParam = tauCalc<T, M>(n_slab, n_slide_top, n_slide_bottom, tc);// tau
                else
                    fixedParam = setG(k); // g

                func<T, M, N, fix> toMinimize(fixedParam, n_slab, n_slide_top, n_slide_bottom, rs, ts);

                int maxIter = 100;
                T astart = 0.5;
                T tstart = 0.1;
                T gstart = 0.5;
                T fmin;
                Matrix<T, 1, N> vecMin;

                NelderMeadMin<T, M, N, fix>(toMinimize, maxIter, astart, tstart, gstart, vecMin, fmin);

                myfileIAD << "TRUE " << setA(i) << " " << setT(j) << " " << setG(k) << " " << rs << " " << ts << " " << tc << std::endl;
                myfileIAD << "CALC " << vecMin(0) << " " << fixedParam << " " << vecMin(1) << std::endl;
            }
        }
    }

    myfileIAD.close();
    //*/

    T ts, rs;
    RTs<T,M>(a, tau, g, n_slab, n_slide_top, n_slide_bottom, v, w, rs, ts);
    std::cout << "T scattered = " << ts << std::endl;
    std::cout << "R scattered = " << rs << std::endl;

    T tc = Tc<T,M>(1.0, n_slab, n_slide_top, n_slide_bottom);
    std::cout << "T collimated = " << tc << std::endl;
    //

    // T rsmeas = 0.139269;
    // T tsmeas = 0.860729;
    // T tcmeas = 5.64959e-006;

    // IAD<T,M,N,fix>(rsmeas,tsmeas,tcmeas,n_slab,n_slide_top,n_slide_bottom);

    return 0;
}
