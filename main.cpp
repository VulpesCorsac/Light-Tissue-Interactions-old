#include <iostream>
#include "IAD/Quadrature.h"
#include "IAD/Angles.h"
#include "IAD/DMmethod.h"
#include "IAD/RedistributionFunction.h"
#include "IAD/LayerInit.h"
#include "IAD/Adding&doubling.h"
#include "IAD/RandT.h"



int main (int argc, char **argv) {

    const int M = 4;


    double a = 0.9; // albedo
    double tau = 1.0; // optical thickness
    double g = 0.9; // anisotropy
    double n_slab = 1.4; // refraction index of sample
    double n_slide_top = 1.5; // refraction index of slide
    double n_slide_bottom = 1.5;

    Quadrature<double, M> quadrature(n_slab);

    array<double, M> v, w;

    v = quadrature.getV();
    w = quadrature.getW();

/*    cout << "Knots" << endl;
    quadrature.printQuadrature(v);
    cout << "Weights" << endl;
    quadrature.printQuadrature(w);*/


    double ts, rs;
    RTs<double, M>(a, tau, g, n_slab, n_slide_top, n_slide_bottom, v, w, rs, ts);
    cout << "T scattered = " << ts << endl;
    cout << "R scattered = " << rs << endl;

    double tc = Tc<double, M>(tau, n_slab, n_slide_top, n_slide_bottom);
    cout << "T collimated = " << tc << endl;


    return 0;

}
