#pragma once

//#include "NelderMead.h"
#include "Quadrature.h"
#include "RT.h"

#include <stdexcept>
#include <utility>

template < typename T, size_t M >
T tauCalc(T n_slab, T n_slide_top, T n_slide_bottom, T Tcol) {
    using namespace Utils_NS;

    const T Rb1 = Rborder<T,M>(n_slab, n_slide_top);
    const T Rb2 = Rborder<T,M>(n_slab, n_slide_bottom);
    const auto cache1 = Rb1 * Rb2;
    const auto cache2 = cache1 - Rb1 - Rb2 + 1;
    return log((sqrt(4 * cache1 * sqr(Tcol) + sqr(cache2)) + cache2) / (2 * Tcol));
}

template < typename T, size_t M >
T funcToMinimize(T a, T tau, T g, T n_slab, T n_slide_top, T n_slide_bottom, T rmeas, T tmeas) {
    Quadrature<T,M> quadrature(n_slab);
    const auto v = quadrature.getV();
    const auto w = quadrature.getW();

    T ts, rs;
    RTs<T,M>(a, tau, g, n_slab, n_slide_top, n_slide_bottom, v, w, rs, ts);
    /// TODO: WHAT IS THIS 1E-6?
    constexpr auto eps = 1E-6;
    return fabs((rs - rmeas)/(rmeas + eps)) + fabs((ts - tmeas)/(tmeas + eps));
}

template < typename T, size_t M, size_t N, bool fix >
class Minimizable {
public:
    virtual T funcToMinimize3args(Matrix<T,1,N> vec) const = 0;
};

template < typename T, size_t M, size_t N, bool fix >
class func: public Minimizable<T,M,N,fix> {
public:
    func(T fixed_param, T n_slab_new, T n_slide_top_new, T n_slide_bottom_new, T rmeas_new, T tmeas_new, T tcmeas_new) noexcept
        : n_slab(n_slab_new)
        , n_slide_top(n_slide_top_new)
        , n_slide_bottom(n_slide_bottom_new)
        , rmeas(rmeas_new)
        , tmeas(tmeas_new)
        , tcmeas(tcmeas_new) {
        if (fix)
            this->tau = fixed_param;
        else
            this->g = fixed_param;
    }

    T funcToMinimize3args(Matrix<T,1,N> vec) const {
        /// ROSENBROCK
        // return 100*sqr(vec(1) - sqr(vec(0))) + sqr(vec(0) - 1) + 100*sqr(vec(2) - sqr(vec(1))) +sqr(vec(1) - 1);
        if (N == 2) {
            if (fix)
                return funcToMinimize<T,M>(vec(0), this->tau, vec(1), this->n_slab, this->n_slide_top, this->n_slide_bottom, this->rmeas, this->tmeas);
            else
                return funcToMinimize<T,M>(vec(0), vec(1), this->g, this->n_slab, this->n_slide_top, this->n_slide_bottom, this->rmeas, this->tmeas);
        } else if (N == 3)
            return funcToMinimize<T,M>(vec(0), vec(1), vec(2), this->n_slab, this->n_slide_top, this->n_slide_bottom, this->rmeas, this->tmeas);
        else
            throw std::invalid_argument("N should be in range [2, 3]");
    }

    T getNslab() const noexcept { return n_slab; }
    T getNslide_top() const noexcept { return n_slide_top; }
    T getNslide_bottom() const noexcept { return n_slide_bottom; }
    T getRmeas() const noexcept { return rmeas; }
    T getTmeas() const noexcept { return tmeas; }
    T getTcmeas() const noexcept { return tcmeas; }
    T getTau() const noexcept { return tau; }
    T getG() const noexcept { return g; }

protected:
    T n_slab, n_slide_top, n_slide_bottom, rmeas, tmeas, tcmeas;
    T tau, g;
};

template < typename T, size_t M, size_t N, bool fix >
T fixParam (T newG, T n_slab, T n_slide_top, T n_slide_bottom, T tcmeas) {
    if (fix) // fixed tau
        return tauCalc<T,M>(n_slab, n_slide_top, n_slide_bottom, tcmeas); // tau
    else // fixed g
        return newG; // g
}

template < typename T, size_t gSize >
void constructGrid(Matrix<T,1,gSize>& gridA, Matrix<T,1,gSize>& gridT, Matrix<T,1,gSize>& gridG) {
    using namespace Utils_NS;

    T tMin = pow(2, -7);
    T tMax = pow(2, +7);
    for (size_t i = 0; i < gSize; i++) {
        T j = i;
        T x = j / (gSize - 1.0);
        if (x < 0.25)
            gridA(i) = 0.9999*(1 - x)+0.00001;
        else if (x > 0.75)
            gridA(i) = 0.9999*(1 - x)+0.00001;
        else
            gridA(i) = 0.9999*sqr(1 - x)+0.00001;
        gridT(i) = exp(tMin + (tMax - tMin) * x);
        gridG(i) = 0.9999 * (2.0 * i / (gSize - 1.0) - 1.0) + 0.00001;

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

template <typename T, size_t M, size_t N, size_t gSize, bool fix>
Matrix<T,gSize,gSize> distances(func<T,M,N,fix> f, Matrix<T,1,gSize> gridA, Matrix<T,1,gSize> gridT, Matrix<T,1,gSize> gridG) {
    Matrix<T,gSize,gSize> dist;

    Quadrature<T,M> quadStart(f.getNslab());
    const auto vStart = quadStart.getV();
    const auto wStart = quadStart.getW();

    for (size_t i = 0; i < gSize; i++)
        for (size_t j = 0; j < gSize; j++) {
            T ts0 = 0;
            T rs0 = 0;
            /// TODO: WHAT IS THIS 1E-6?
            constexpr auto eps = 1E-6;
            if (fix) { // tau fixed
                RTs<T,M>(gridA(i), f.getTau(), gridG(j), f.getNslab(), f.getNslide_top(), f.getNslide_bottom(), vStart, wStart, rs0, ts0);
                dist(i,j) = std::abs(rs0 - f.getRmeas()) / (f.getRmeas() + eps) + std::abs(ts0 - f.getTmeas()) / (f.getTmeas() + eps);
            } else {
                RTs<T,M>(gridA(i), gridT(j), f.getG(), f.getNslab(), f.getNslide_top(), f.getNslide_bottom(), vStart, wStart, rs0, ts0);
                dist(i,j) = std::abs(rs0 - f.getRmeas()) / (f.getRmeas() + eps) + std::abs(ts0 - f.getTmeas()) / (f.getTmeas() + eps);
            }
        }

    return dist;
}

template < typename T, size_t M, size_t N, bool fix >
void startingPoints(func<T,M,N,fix> f, T& aStart, T& tStart, T& gStart) {
    constexpr int gridSize = 20;
    Matrix<T, 1, gridSize> gridA, gridT, gridG;

    constructGrid<T,gridSize>(gridA, gridT, gridG);
    Matrix<T,gridSize,gridSize> distancesMatrix = distances<T,M,N,gridSize,fix>(f, gridA, gridT, gridG);

    // std::cout << distancesMatrix << std::endl;

    int minRow, minCol;
    /// TODO: Unused variable?
    T mins = distancesMatrix.minCoeff(&minRow, &minCol);
    /*
    std::cout << minRow << " " << minCol << " " << distancesMatrix.minCoeff() << std::endl;
    std::cout << gridA << std::endl;
    std::cout << gridG << std::endl;
    //*/

    if (fix) {
        aStart = gridA(minRow);
        gStart = gridG(minCol);
        tStart = f.getTau();
    } else {
        aStart = gridA(minRow);
        tStart = gridT(minCol);
        gStart = f.getG();
    }
}
