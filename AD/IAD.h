#pragma once

#include <utility>

#include "Quadrature.h"
#include "RT.h"

template <typename T, size_t M>
T tauCalc (T n_slab, T n_slide_top, T n_slide_bottom, T Tcol){
    T Rb1 = Rborder<T, M>(n_slab, n_slide_top);
    T Rb2 = Rborder<T, M>(n_slab, n_slide_bottom);
    return log((sqrt(4 * Rb1 * Rb2 * sqr(Tcol) + sqr(Rb1 * Rb2 - Rb1 - Rb2 + 1)) + Rb1 * Rb2 - Rb1 - Rb2 + 1)/(2 * Tcol));
}

template <typename T, size_t M>
T funcToMinimize(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom, T rmeas, T tmeas) {
    Quadrature<T,M> quadrature(n_slab);
    const auto v = quadrature.getV();
    const auto w = quadrature.getW();

    T ts, rs;
    RTs<T,M>(a, tau, g, n_slab, n_slide_top, n_slide_bottom, v, w, rs, ts);

    return fabs((rs - rmeas)/(rmeas + 1E-6)) + fabs((ts - tmeas)/(tmeas + 1E-6));
}

template <typename T, size_t M, size_t N>
class Minimizable {
public:
    virtual T funcToMinimize3args(Matrix<T, 1, N> vec) = 0;
};

template <typename T, size_t M, size_t N>
class func: public Minimizable<T, M, N> {
public:
    func (T n_slab_new, T n_slide_top_new, T n_slide_bottom_new, T rmeas_new, T tmeas_new, T tcmeas_new) {
        this->n_slab = n_slab_new;
        this->n_slide_top = n_slide_top_new;
        this->n_slide_bottom = n_slide_bottom_new;
        this->rmeas = rmeas_new;
        this->tmeas = tmeas_new;
        this->tau = tauCalc<T, M>(n_slab_new, n_slide_top_new, n_slide_bottom_new, tcmeas_new);
    }
    T funcToMinimize3args (Matrix<T, 1, N> vec) {
        //ROSENBROCK
        //return 100*sqr(vec(1) - sqr(vec(0))) + sqr(vec(0) - 1) + 100*sqr(vec(2) - sqr(vec(1))) +sqr(vec(1) - 1);
        if (N == 2)
            return funcToMinimize<T, M>(vec(0), this->tau, vec(1), this->n_slab, this->n_slide_top, this->n_slide_bottom, this->rmeas, this->tmeas);
        else if (N == 3)
            return funcToMinimize<T, M>(vec(0), vec(1), vec(2), this->n_slab, this->n_slide_top, this->n_slide_bottom, this->rmeas, this->tmeas);
    }
protected:
    T n_slab, n_slide_top, n_slide_bottom, rmeas, tmeas;
    T tau;
};





