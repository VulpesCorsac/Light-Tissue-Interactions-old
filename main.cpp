#include "AD/Quadrature.h"
#include "AD/RT.h"

#include <iostream>

int main (int argc, char **argv) {
    using T = double;
    const int M = 4;

    T a = 0.9; // albedo
    T tau = 1.0; // optical thickness
    T g = 0.9; // anisotropy
    T n_slab = 1.4; // refraction index of sample
    T n_slide_top = 1.5; // refraction index of slide
    T n_slide_bottom = 1.5;

    Quadrature<T,M> quadrature(n_slab);

    const auto v = quadrature.getV();
    const auto w = quadrature.getW();

    /*
    std::cout << "Knots" << std::endl;
    quadrature.printQuadrature(v);
    std::cout << "Weights" << std::endl;
    quadrature.printQuadrature(w);
    //*/


    T ts, rs;
    RTs<T,M>(a, tau, g, n_slab, n_slide_top, n_slide_bottom, v, w, rs, ts);
    std::cout << "T scattered = " << ts << std::endl;
    std::cout << "R scattered = " << rs << std::endl;

    T tc = Tc<T,M>(tau, n_slab, n_slide_top, n_slide_bottom);
    std::cout << "T collimated = " << tc << std::endl;

    return 0;
}
