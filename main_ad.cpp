// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "AD/Quadrature.h"
#include "AD/RT.h"
#include "AD/NelderMead.h"

#include <iostream>

int main (int argc, char **argv) {
    using T = float;
    const int M = 4;

    T a = 0.9; // albedo
    T tau = 1.0; // optical thickness
    T g = 0.9; // anisotropy
    T n_slab = 1.5; // refraction index of sample
    T n_slide_top = 1.6; // refraction index of slide
    T n_slide_bottom = 1.6;

/*    Quadrature<T,M> quadrature(n_slab);

    const auto v = quadrature.getV();
    const auto w = quadrature.getW();*/

    /*
    std::cout << "Knots" << std::endl;
    quadrature.printQuadrature(v);
    std::cout << "Weights" << std::endl;
    quadrature.printQuadrature(w);
    //*/


 /*   T ts, rs;
    RTs<T,M>(a, tau, g, n_slab, n_slide_top, n_slide_bottom, v, w, rs, ts);
    std::cout << "T scattered = " << ts << std::endl;
    std::cout << "R scattered = " << rs << std::endl;

    T tc = Tc<T,M>(tau, n_slab, n_slide_top, n_slide_bottom);
    std::cout << "T collimated = " << tc << std::endl;*/

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    T rsmeas = 0.389;
    T tsmeas = 0.119;
    T tcmeas = 0.0064;


    T fixedParam;// g or tau
    if (fix == 1){
        fixedParam = tauCalc<T, M>(n_slab, n_slide_top, n_slide_bottom, tcmeas);// tau
        std::cout << "Tau = " << fixedParam << std::endl;
    }
    else
        fixedParam = 0.0; // g

    func<T, M, N, fix> toMinimize(fixedParam, n_slab, n_slide_top, n_slide_bottom, rsmeas, tsmeas);

    int maxIter = 100;
    T astart = 0.9;
    T tstart = 0.1;
    T gstart = 0.3;
    T fmin;
    Matrix<T, 1, N> vecMin;

    NelderMeadMin<T, M, N, fix>(toMinimize, maxIter, astart, tstart, gstart, vecMin, fmin);

    if (fix == 1)
        std::cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", g = " << vecMin(1) << std::endl;
    else
        std::cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << std::endl;

    return 0;
}
