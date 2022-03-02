#pragma once

//#include "NelderMead.h"
#include "Quadrature.h"
#include "RT.h"

#include "../Minimization/FixedParam.h"
#include "../Utils/Contracts.h"

#include <utility>

template < typename T, size_t M >
T tauCalc(T nSlab, T nSlideTop, T nSlideBottom, T Tcol) {
    using namespace Math_NS;

    const auto Rb1 = Rborder<T,M>(nSlab, nSlideTop);
    const auto Rb2 = Rborder<T,M>(nSlab, nSlideBottom);
    const auto cache1 = Rb1 * Rb2;
    const auto cache2 = cache1 - Rb1 - Rb2 + 1;

    CHECK_ARGUMENT_CONTRACT(Tcol != 0);
    CHECK_RUNTIME_CONTRACT(4 * cache1 * sqr(Tcol) + sqr(cache2) > 0);
    CHECK_RUNTIME_CONTRACT((sqrt(4 * cache1 * sqr(Tcol) + sqr(cache2)) + cache2) / (2 * Tcol) > 0);

    return log((sqrt(4 * cache1 * sqr(Tcol) + sqr(cache2)) + cache2) / (2 * Tcol));
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

    CHECK_ARGUMENT_CONTRACT(rmeas + eps > 0);
    CHECK_ARGUMENT_CONTRACT(tmeas + eps > 0);

    return abs((rs - rmeas) / (rmeas + eps)) + abs((ts - tmeas) / (tmeas + eps));
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
    Func(T fixedParam, T nSlabNew, T nSlideTopNew, T nSlideBottomNew, T rmeasNew, T tmeasNew, T tcmeasNew) EXCEPT_INPUT_PARAMS
        : nSlab(nSlabNew)
        , nSlideTop(nSlideTopNew)
        , nSlideBottom(nSlideBottomNew)
        , rmeas(rmeasNew)
        , tmeas(tmeasNew)
        , tcmeas(tcmeasNew) {
        using namespace Minimization_NS;

        CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

        if (fix == FixedParameter::Tau)
            this->tau = fixedParam;
        else if (fix == FixedParameter::G)
            this->g = fixedParam;
    }

    T funcToMinimize3args(Matrix<T,1,N> vec) const {
        using namespace Minimization_NS;

        /// ROSENBROCK
        // return 100*sqr(vec(1) - sqr(vec(0))) + sqr(vec(0) - 1) + 100*sqr(vec(2) - sqr(vec(1))) +sqr(vec(1) - 1);

        CHECK_ARGUMENT_CONTRACT(2 <= N && N <= 3);

        if (N == 2) {
            CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

            if (fix == FixedParameter::Tau)
                return funcToMinimize<T,M>(vec(0), this->tau, vec(1), this->nSlab, this->nSlideTop, this->nSlideBottom, this->rmeas, this->tmeas);
            else if (fix == FixedParameter::G)
                return funcToMinimize<T,M>(vec(0), vec(1), this->g, this->nSlab, this->nSlideTop, this->nSlideBottom, this->rmeas, this->tmeas);
        } else if (N == 3)
            return funcToMinimize<T,M>(vec(0), vec(1), vec(2), this->nSlab, this->nSlideTop, this->nSlideBottom, this->rmeas, this->tmeas);
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

    CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

    if (fix == FixedParameter::Tau)
        return tauCalc<T,M>(nSlab, nSlideTop, nSlideBottom, tcmeas);
    else if (fix == FixedParameter::G)
        return newG;
}

template < typename T, size_t N, size_t gSize, Minimization_NS::FixedParameter fix >
void constructGrid(Matrix<T,1,gSize>& gridA, Matrix<T,1,gSize>& gridT, Matrix<T,1,gSize>& gridG) {
    using namespace Minimization_NS;
    T tMin = -5;
    T tMax = 3;
    if ((fix == FixedParameter::G && N == 2) || N == 3) {
        std::cout << "Thick (tau > 2) or thin (tau < 2)? Thick = 0, thin = 1" << std::endl;
        int thick;
        std::cin >> thick;
        if (thick == 0) {
            tMin = 0;
            tMax = 4;
        } else if (thick == 1) {
            tMin = -5;
            tMax = 0;
        }
    }
    for (size_t i = 0; i < gSize; i++) {
        const T j = i;
        const T x = j / (gSize - 1.0);
        if (x < 0.25)
            gridA(i) = 0.9999 * (1 - x) + 0.00001;
        else if (x > 0.75)
            gridA(i) = 0.9999 * (1 - x) + 0.00001;
        else
            gridA(i) = 0.9999 * Math_NS::sqr(1 - x) + 0.00001;
        gridT(i) = pow(2, tMin + (tMax - tMin) * x);
//        gridG(i) = 0.9999 * (2.0 * i / (gSize - 1.0) - 1.0) + 0.00001;
        gridG(i) = 0.9999 * i / (gSize - 1.0) + 0.00001;
  //      std::cout << gridA(i) << " " << gridG(i) << std::endl;
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
Matrix<T,gSize,gSize> distances(const Func<T,M,N,fix>& f, const Matrix<T,1,gSize>& gridA, const Matrix<T,1,gSize>& gridT, const Matrix<T,1,gSize>& gridG, const T& g) {
    using namespace Minimization_NS;
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

    Matrix<T,gSize,gSize> dist;

    Quadrature<T,M> quadStart(f.getNslab());
    const auto vStart = quadStart.getV();
    const auto wStart = quadStart.getW();

    if (N == 2) {
        T ts0 = 0;
        T rs0 = 0;
        constexpr auto eps = 1E-6;
        RTs<T,M>(0.9, 1.0, 0.9, f.getNslab(), f.getNslideTop(), f.getNslideBottom(), vStart, wStart, rs0, ts0);
       // cout << f.getNslab() << " " << f.getNslideTop() << " " << f.getNslideBottom() << " " << f.getRmeas() << " " << f.getTmeas() << endl;
       // cout << rs0 << " " << ts0 << endl;
       // cout << abs(rs0 - f.getRmeas()) + abs(ts0 - f.getTmeas()) << endl;
       for (size_t i = 0; i < gSize; i++) {
        for (size_t j = 0; j < gSize; j++) {
            if (fix == FixedParameter::Tau) {
                RTs<T,M>(gridA(i), f.getTau(), gridG(j), f.getNslab(), f.getNslideTop(), f.getNslideBottom(), vStart, wStart, rs0, ts0);
                dist(i,j) = abs(rs0 - (f.getRmeas()+0.02)) / ((f.getRmeas()+0.02) + eps) + abs(ts0 - (f.getTmeas())) / ((f.getTmeas()) + eps);
              //  cout << "a = " << gridA(i) << " g = " << gridG(j) << " tau = " << f.getTau() << " : " << dist(i,j) << endl;
            } else if (fix == FixedParameter::G) {
                RTs<T,M>(gridA(i), gridT(j), f.getG(), f.getNslab(), f.getNslideTop(), f.getNslideBottom(), vStart, wStart, rs0, ts0);
                dist(i,j) = abs(rs0 - (f.getRmeas()+0.02)) / ((f.getRmeas()+0.02) + eps) + abs(ts0 - (f.getTmeas())) / ((f.getTmeas()) + eps);
              //  cout << dist(i,j) << " ";
            }
        }
       }
    } else if (N == 3) {
        constexpr auto eps = 1E-6;
        T ts0 = 0;
        T rs0 = 0;
        //RTs<T,M>(0.9, 1.0, 0.9, f.getNslab(), f.getNslideTop(), f.getNslideBottom(), vStart, wStart, rs0, ts0);
        cout << abs(rs0 - f.getRmeas()) + abs(ts0 - f.getTmeas()) << endl;
      for (size_t i = 0; i < gSize; i++) {
        for (size_t j = 0; j < gSize; j++) {
            /// TODO: WHAT IS THIS 1E-6?
            RTs<T,M>(gridA(i), gridT(j), g, f.getNslab(), f.getNslideTop(), f.getNslideBottom(), vStart, wStart, rs0, ts0);
            dist(i,j) = abs(rs0 - (f.getRmeas()+0.02)) / ((f.getRmeas()+0.02) + eps) + abs(ts0 - (f.getTmeas()+0.02)) / ((f.getTmeas()+0.02) + eps);
            //dist(i,j) = std::abs(rs0 - f.getRmeas()) + std::abs(ts0 - f.getTmeas());
        }
      }
    }
  //  cout << "wow" << endl;
    return dist;
}

template < typename T, size_t M, size_t N, Minimization_NS::FixedParameter fix >
void startingPoints(const Func<T,M,N,fix>& f, T& aStart, T& tStart, T& gStart) {
    using namespace Minimization_NS;

    CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

    constexpr int gridSize = 20;
    Matrix<T, 1, gridSize> gridA, gridT, gridG;
    constructGrid<T, N, gridSize, fix>(gridA, gridT, gridG);
    if (N == 2) {
        const auto distancesMatrix = distances<T,M,N,gridSize,fix>(f, gridA, gridT, gridG, 0.0);
        int minRow, minCol;
        const T mins = distancesMatrix.minCoeff(&minRow, &minCol);
        std::ignore = mins;
     //   std::cout << "?" << std::endl;
        /*
        std::cout << minRow << " " << minCol << " " << distancesMatrix.minCoeff() << std::endl;
        std::cout << gridA << std::endl;
        std::cout << gridG << std::endl;
        //*/
        if (fix == FixedParameter::Tau) {
            aStart = gridA(minRow);
            gStart = gridG(minCol);
            tStart = f.getTau();
            std::cout << f.getTau() << ": " << distancesMatrix.minCoeff() << " " << minRow << " " << minCol << std::endl;
            std::cout << gridA(minRow) << " " << gridG(minCol) << std::endl;
        } else if (fix == FixedParameter::G) {
            aStart = gridA(minRow);
            tStart = gridT(minCol);
            gStart = f.getG();
            std::cout << f.getG() << ": " << distancesMatrix.minCoeff() << " " << minRow << " " << minCol << std::endl;
        }
        std::cout << aStart << " " << gStart << " " << tStart << std::endl;
    } else if (N == 3) {
        std::vector<T> vectorMins;
        std::vector<std::pair<int,int>> vectorCoord;
        for (int i = 0; i < gridSize; i++) {
            const auto distancesMatrix = distances<T,M,N,gridSize,fix>(f, gridA, gridT, gridG, gridG(i));
            int minRow, minCol;
            const T mins = distancesMatrix.minCoeff(&minRow, &minCol);
            const T minHere = distancesMatrix.minCoeff();
            vectorMins.push_back(minHere);
            vectorCoord.push_back(std::make_pair(minRow, minCol));
            std::ignore = mins;
            std::cout << gridG(i) << " " << gridA(minRow) << " " << gridT(minCol) << ": " << minHere << std::endl;
        }
        int minElementIndex = std::min_element(vectorMins.begin(),vectorMins.end()) - vectorMins.begin();
        aStart = gridA(vectorCoord[minElementIndex].first);
        tStart = gridT(vectorCoord[minElementIndex].second);
        gStart = gridG(minElementIndex);
    }
}
