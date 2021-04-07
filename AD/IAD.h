#pragma once

#include <utility>

//#include "NelderMead.h"
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

template <typename T, size_t M, size_t N, bool fix>
class Minimizable {
public:
    virtual T funcToMinimize3args(Matrix<T, 1, N> vec) = 0;
};

template <typename T, size_t M, size_t N, bool fix>
class func: public Minimizable<T, M, N, fix> {
public:
    func (T fixed_param, T n_slab_new, T n_slide_top_new, T n_slide_bottom_new, T rmeas_new, T tmeas_new, T tcmeas_new) {
        this->n_slab = n_slab_new;
        this->n_slide_top = n_slide_top_new;
        this->n_slide_bottom = n_slide_bottom_new;
        this->rmeas = rmeas_new;
        this->tmeas = tmeas_new;
        this->tcmeas = tcmeas_new;
        if (fix == 1)
            this->tau = fixed_param;
        else
            this->g = fixed_param;
    }
    T funcToMinimize3args (Matrix<T, 1, N> vec) {
        //ROSENBROCK
        //return 100*sqr(vec(1) - sqr(vec(0))) + sqr(vec(0) - 1) + 100*sqr(vec(2) - sqr(vec(1))) +sqr(vec(1) - 1);
        if (N == 2){
            if (fix == 1)
                return funcToMinimize<T, M>(vec(0), this->tau, vec(1), this->n_slab, this->n_slide_top, this->n_slide_bottom, this->rmeas, this->tmeas);
            else
                return funcToMinimize<T, M>(vec(0), vec(1), this->g, this->n_slab, this->n_slide_top, this->n_slide_bottom, this->rmeas, this->tmeas);
        }
        else if (N == 3)
            return funcToMinimize<T, M>(vec(0), vec(1), vec(2), this->n_slab, this->n_slide_top, this->n_slide_bottom, this->rmeas, this->tmeas);
    }
    T getNslab() const {return n_slab;}
    T getNslide_top() const {return n_slide_top;}
    T getNslide_bottom() const {return n_slide_bottom;}
    T getRmeas() const {return rmeas;}
    T getTmeas() const {return tmeas;}
    T getTcmeas() const {return tcmeas;}
    T getTau() const {return tau;}
    T getG() const {return g;}
protected:
    T n_slab, n_slide_top, n_slide_bottom, rmeas, tmeas, tcmeas;
    T tau, g;
};

template <typename T, size_t M, size_t N, bool fix>
T fixParam (T newG, T n_slab, T n_slide_top, T n_slide_bottom, T tcmeas) {
    if (fix == 1) // fixed tau
        return tauCalc<T, M>(n_slab, n_slide_top, n_slide_bottom, tcmeas);// tau
    else // fixed g
        return newG; //g
}

template <typename T, size_t gSize>
void constructGrid(Matrix<T, 1, gSize>& gridA, Matrix<T, 1, gSize>& gridT, Matrix<T, 1, gSize>& gridG) {
    T tMin = pow(2, -7);
    T tMax = pow(2, 7);
    for (int i = 0; i < gSize; i++) {
        T j = i;
        T x = j / (gSize - 1.0);
        if (x < 0.25)
            gridA(i) = 1 - x;
        else if (x > 0.75)
            gridA(i) = 1 - x;
        else
            gridA(i) = sqr(1 - x);
        gridT(i) = exp(tMin + (tMax - tMin) * x);
        gridG(i) = 0.9999 * i / (gSize - 1);
    }
}

template <typename T, size_t M, size_t N, size_t gSize, bool fix>
Matrix<T, gSize, gSize> distances (func<T, M, N, fix> f, Matrix<T, 1, gSize> gridA, Matrix<T, 1, gSize> gridT, Matrix<T, 1, gSize> gridG) {

    Matrix<T, gSize, gSize> dist;

    Quadrature<T,M> quadStart(f.getNslab());
    const auto vStart = quadStart.getV();
    const auto wStart = quadStart.getW();

       for (int i = 0; i < gSize; i++){
        for (int j = 0; j < gSize; j++) {
            T ts0 = 0;
            T rs0 = 0;
            if (fix == 1) {// tau fixed
                RTs<T,M>(gridA(i), f.getTau(), gridG(j), f.getNslab(), f.getNslide_top(), f.getNslide_bottom(), vStart, wStart, rs0, ts0);
                dist(i,j) = std::abs(rs0 - f.getRmeas())/(f.getRmeas() + 1E-6) + std::abs(ts0 - f.getTmeas())/(f.getTmeas() + 1E-6);
            } else {
                RTs<T,M>(gridA(i), gridT(j), f.getG(), f.getNslab(), f.getNslide_top(), f.getNslide_bottom(), vStart, wStart, rs0, ts0);
                dist(i,j) = std::abs(rs0 - f.getRmeas())/(f.getRmeas() + 1E-6) + std::abs(ts0 - f.getTmeas())/(f.getTmeas() + 1E-6);
            }
        }
    }
    return dist;
}

template <typename T, size_t M, size_t N, bool fix>
void startingPoints(func<T, M, N, fix> f, T& aStart, T& tStart, T& gStart) {
    const int gridSize = 15;
    Matrix<T, 1, gridSize> gridA, gridT, gridG;

    constructGrid<T,gridSize>(gridA, gridT, gridG);
    Matrix<T, gridSize, gridSize> distancesMatrix = distances<T, M, N, gridSize, fix>(f, gridA, gridT, gridG);

    int minRow, minCol;
    T mins = distancesMatrix.minCoeff(&minRow, &minCol);

    if (fix == 1){
        aStart = gridA(minRow);
        gStart = gridG(minCol);
        tStart = f.getTau();
    } else {
        aStart = gridA(minRow);
        tStart = gridT(minCol);
        gStart = f.getG();
    }
}





