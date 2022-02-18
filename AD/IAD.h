#pragma once

//#include "NelderMead.h"
#include "Quadrature.h"
#include "RT.h"

#include "../Minimization/FixedParam.h"

#include <stdexcept>
#include <utility>

template < typename T, size_t M >
T tauCalc(T nSlab, T nSlideTop, T nSlideBottom, T Tcol) {
    const auto Rb1 = Rborder<T,M>(nSlab, nSlideTop);
    const auto Rb2 = Rborder<T,M>(nSlab, nSlideBottom);
    const auto cache1 = Rb1 * Rb2;
    const auto cache2 = cache1 - Rb1 - Rb2 + 1;
    return log((sqrt(4 * cache1 * Math_NS::sqr(Tcol) + Math_NS::sqr(cache2)) + cache2) / (2 * Tcol));
}

template < typename T, size_t M >
T funcToMinimize(T a, T tau, T g, T nSlab, T nSlideTop, T nSlideBottom, T rmeas, T tmeas) {
    Quadrature<T,M> quadrature(nSlab);
    const auto v = quadrature.getV();
    const auto w = quadrature.getW();

    T ts;
    T rs;
    RTs<T,M>(a, tau, g, nSlab, nSlideTop, nSlideBottom, v, w, rs, ts);
    /// TODO: WHAT IS THIS 1E-6?
    constexpr auto eps = 1E-6;
    return fabs((rs - rmeas) / (rmeas + eps)) + fabs((ts - tmeas) / (tmeas + eps));
}

template < typename T, size_t M, size_t N, Minimization_NS::FixedParameter fix >
class Minimizable {
public:
    virtual T funcToMinimize3args(Matrix<T,1,N> vec) const = 0;

    virtual ~Minimizable() = default;
};

template < typename T, size_t M, size_t N, Minimization_NS::FixedParameter fix >
class Func : public Minimizable<T,M,N,fix> {
public:
    Func(T fixedParam, T nSlabNew, T nSlideTopNew, T nSlideBottomNew, T rmeasNew, T tmeasNew, T tcmeasNew)
        : nSlab(nSlabNew)
        , nSlideTop(nSlideTopNew)
        , nSlideBottom(nSlideBottomNew)
        , rmeas(rmeasNew)
        , tmeas(tmeasNew)
        , tcmeas(tcmeasNew) {
        using namespace Minimization_NS;

        if (fix == FixedParameter::Tau)
            this->tau = fixedParam;
        else if (fix == FixedParameter::G)
            this->g = fixedParam;
        else
            throw std::invalid_argument("Need to have fixed parameter");
    }

    T funcToMinimize3args(Matrix<T,1,N> vec) const {
        using namespace Minimization_NS;

        /// ROSENBROCK
        // return 100*sqr(vec(1) - sqr(vec(0))) + sqr(vec(0) - 1) + 100*sqr(vec(2) - sqr(vec(1))) +sqr(vec(1) - 1);
        if (N == 2) {
            if (fix == FixedParameter::Tau)
                return funcToMinimize<T,M>(vec(0), this->tau, vec(1), this->nSlab, this->nSlideTop, this->nSlideBottom, this->rmeas, this->tmeas);
            else if (fix == FixedParameter::G)
                return funcToMinimize<T,M>(vec(0), vec(1), this->g, this->nSlab, this->nSlideTop, this->nSlideBottom, this->rmeas, this->tmeas);
            else
                throw std::invalid_argument("Nothing to fix but N == 2");
        } else if (N == 3)
            return funcToMinimize<T,M>(vec(0), vec(1), vec(2), this->nSlab, this->nSlideTop, this->nSlideBottom, this->rmeas, this->tmeas);
        else
            throw std::invalid_argument("N should be in range [2, 3]");
    }

    T getNslab()         const noexcept { return nSlab;        }
    T getNslideTop()     const noexcept { return nSlideTop;    }
    T getNslideBottom()  const noexcept { return nSlideBottom; }
    T getRmeas()         const noexcept { return rmeas;        }
    T getTmeas()         const noexcept { return tmeas;        }
    T getTcmeas()        const noexcept { return tcmeas;       }
    T getTau()           const noexcept { return tau;          }
    T getG()             const noexcept { return g;            }

protected:
    T nSlab, nSlideTop, nSlideBottom, rmeas, tmeas, tcmeas;
    T tau, g;
};

template < typename T, size_t M, size_t N, Minimization_NS::FixedParameter fix >
T fixParam(T newG, T nSlab, T nSlideTop, T nSlideBottom, T tcmeas) {
    using namespace Minimization_NS;

    if (fix == FixedParameter::Tau)
        return tauCalc<T,M>(nSlab, nSlideTop, nSlideBottom, tcmeas);
    else if (fix == FixedParameter::G)
        return newG;
    else
        throw std::invalid_argument("Need to have fixed parameter");
}

template < typename T, size_t gSize >
void constructGrid(Matrix<T,1,gSize>& gridA, Matrix<T,1,gSize>& gridT, Matrix<T,1,gSize>& gridG) {
    const T tMin = pow(2, -7);
    const T tMax = pow(2, +7);
    for (size_t i = 0; i < gSize; i++) {
        const T j = i;
        const T x = j / (gSize - 1.0);
        if (x < 0.25)
            gridA(i) = 0.9999 * (1 - x) + 0.00001;
        else if (x > 0.75)
            gridA(i) = 0.9999 * (1 - x) + 0.00001;
        else
            gridA(i) = 0.9999 * Math_NS::sqr(1 - x) + 0.00001;
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

template < typename T, size_t M, size_t N, size_t gSize, Minimization_NS::FixedParameter fix >
Matrix<T,gSize,gSize> distances(Func<T,M,N,fix> f, Matrix<T,1,gSize> gridA, Matrix<T,1,gSize> gridT, Matrix<T,1,gSize> gridG) {
    using namespace Minimization_NS;

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
            if (fix == FixedParameter::Tau) {
                RTs<T,M>(gridA(i), f.getTau(), gridG(j), f.getNslab(), f.getNslideTop(), f.getNslideBottom(), vStart, wStart, rs0, ts0);
                dist(i,j) = std::abs(rs0 - f.getRmeas()) / (f.getRmeas() + eps) + std::abs(ts0 - f.getTmeas()) / (f.getTmeas() + eps);
            } else if (fix == FixedParameter::G) {
                RTs<T,M>(gridA(i), gridT(j), f.getG(), f.getNslab(), f.getNslideTop(), f.getNslideBottom(), vStart, wStart, rs0, ts0);
                dist(i,j) = std::abs(rs0 - f.getRmeas()) / (f.getRmeas() + eps) + std::abs(ts0 - f.getTmeas()) / (f.getTmeas() + eps);
            } else
                throw std::invalid_argument("Need to have fixed parameter");
        }

    return dist;
}

template < typename T, size_t M, size_t N, Minimization_NS::FixedParameter fix >
void startingPoints(Func<T,M,N,fix> f, T& aStart, T& tStart, T& gStart) {
    using namespace Minimization_NS;

    constexpr int gridSize = 20;
    Matrix<T, 1, gridSize> gridA, gridT, gridG;

    constructGrid<T,gridSize>(gridA, gridT, gridG);
    const auto distancesMatrix = distances<T,M,N,gridSize,fix>(f, gridA, gridT, gridG);

    // std::cout << distancesMatrix << std::endl;

    int minRow, minCol;
    const T mins = distancesMatrix.minCoeff(&minRow, &minCol);
    std::ignore = mins;
    /*
    std::cout << minRow << " " << minCol << " " << distancesMatrix.minCoeff() << std::endl;
    std::cout << gridA << std::endl;
    std::cout << gridG << std::endl;
    //*/

    if (fix == FixedParameter::Tau) {
        aStart = gridA(minRow);
        gStart = gridG(minCol);
        tStart = f.getTau();
    } else if (fix == FixedParameter::G) {
        aStart = gridA(minRow);
        tStart = gridT(minCol);
        gStart = f.getG();
    } else
        throw std::invalid_argument("Need to have fixed parameter");
}
