#pragma once

#include "MonteCarlo.h"
#include "MonteCarloMultithread.h"

#include "../Inverse/Helpers.h"
#include "../Inverse/FixedParam.h"
#include "../Inverse/NelderMead.h"
#include "../Physics/Angles.h"
#include "../Utils/Utils.h"

#include <stdexcept>
#include <utility>

using namespace Inverse_NS;

template < typename T >
T tauCalc(T nSlab, T nSlideTop, T nSlideBottom, T Tcol) {
    using namespace Math_NS;
    using namespace Physics_NS;

    const auto rb1 = BorderReflectance<T>(nSlab, nSlideTop   );
    const auto rb2 = BorderReflectance<T>(nSlab, nSlideBottom);
    const auto cached1 = rb1 * rb2;
    const auto cached2 = cached1 - rb1 - rb2 + 1;
    return log((sqrt(4 * cached1 * sqr(Tcol) + sqr(cached2)) + cached2) / (2 * Tcol));
}

template < typename T, size_t Nz, size_t Nr, bool detector >
T funcToMinimizeMC(const T& a,
                   const T& tau,
                   const T& g,
                   const Medium<T>& emptyTissue,
                   const std::vector<Medium<T>>& slides,
                   int Np,
                   int threads,
                   T z,
                   T r,
                   const IntegratingSphere<T>& sphereR,
                   const IntegratingSphere<T>& sphereT,
                   const DetectorDistance<T> dist,
                   const std::vector<std::pair<T,T>>& rmeas,
                   const std::vector<std::pair<T,T>>& tmeas) {
    using namespace Utils_NS;
    using namespace std;

    // auto tissue = Medium<T>::fromAlbedo(emptyTissue.n, a, tau, emptyTissue.D, g);
    vector<Medium<T>> layers;
    if (slides.empty())
        layers = { Medium<T>::fromAlbedo(emptyTissue.n, a, tau, emptyTissue.D, g) };
    else
        layers = { slides[0], Medium<T>::fromAlbedo(emptyTissue.n, a, tau, emptyTissue.D, g), slides[1] };
    Sample<T> sample(layers, 1.0, 1.0);

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(sample, Np, threads, z, r, myResultsMT, sphereR, sphereT, dist);

    const auto rMC = myResultsMT.detectedR;
    const auto tMC = myResultsMT.detectedT;
    T func2min = 0;
    /*
    cout << "rMC" << endl;
    for (auto x: rMC)
        cout << x.first << " " << x.second << endl;
    cout << "tMC" << endl;
    for (auto x: tMC)
        cout << x.first << " " << x.second << endl;
    //*/
    // constexpr auto EPS = 1E-6;
    for (int i = 0; i < isize(rMC); i++)
        func2min += abs((rMC[i].second - rmeas[i].second)/* / (rmeas[i].second + EPS)*/) + abs((tMC[i].second - tmeas[i].second)/* / (tmeas[i].second + EPS)*/);

    return func2min;
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, Inverse_NS::FixedParameter fix >
class MinimizableMC {
public:
    virtual T funcToMinimize3argsMC(Matrix<T,1,N> vec) const = 0;

    virtual ~MinimizableMC() = default;
};

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, Inverse_NS::FixedParameter fix >
class FuncMC : public MinimizableMC<T,Nz,Nr,detector,N,fix> {
public:
    FuncMC(T fixedParam, const Medium<T>& emptyTissue, const std::vector<Medium<T>>& slides, int Np,
           int threads, T z, T r, const IntegratingSphere<T>& sphereR, const IntegratingSphere<T>& sphereT,
           const DetectorDistance<T> dist, const std::vector<std::pair<T,T>>& rmeas, const std::vector<std::pair<T,T>>& tmeas, const T& tcmeas)
        : emptyTissue(emptyTissue)
        , slides(slides)
        , Np(Np)
        , threads(threads)
        , z(z)
        , r(r)
        , SphereR(sphereR)
        , SphereT(sphereT)
        , dist(dist)
        , rmeas(rmeas)
        , tmeas(tmeas)
        , tcmeas(tcmeas) {
        using namespace Inverse_NS;
        using namespace std;

        if (fix == FixedParameter::Tau)
            this->tau = fixedParam;
        else if (fix == FixedParameter::G)
            this->g = fixedParam;
        else
            throw invalid_argument("Need to have fixed parameter");
    }

    T funcToMinimize3argsMC(Matrix<T,1,N> vec) const {
        using namespace Inverse_NS;
        using namespace std;

        if (N == 2) {
            if (fix == FixedParameter::Tau)
                return funcToMinimizeMC<T,Nz,Nr,detector>(vec(0), this->tau, vec(1), this->emptyTissue, this->slides, this->Np, this->threads, this->z, this->r, this->SphereR, this->SphereT, this->dist, this->rmeas, this->tmeas);
            else if (fix == FixedParameter::G)
                return funcToMinimizeMC<T,Nz,Nr,detector>(vec(0), vec(1), this->g, this->emptyTissue, this->slides, this->Np, this->threads, this->z, this->r, this->SphereR, this->SphereT, this->dist, this->rmeas, this->tmeas);
            else
                throw invalid_argument("Need to have fixed parameter");
        } else if (N == 3)
            return funcToMinimizeMC<T,Nz,Nr,detector>(vec(0), vec(1), vec(2), this->emptyTissue, this->slides, this->Np, this->threads, this->z, this->r, this->SphereR, this->SphereT, this->dist, this->rmeas, this->tmeas);
        else
            throw std::invalid_argument("N should be in range [2, 3]");
    }

    Medium<T> getEmptyTissue() const noexcept { return emptyTissue; }
    std::vector<Medium<T>> getSlides() const noexcept { return slides; }
    int getNp() const noexcept { return Np; }
    int getThreads() const noexcept { return threads; }
    T getZ() const noexcept { return z; }
    T getR() const noexcept { return r; }
    IntegratingSphere<T> getSphereR() const noexcept { return SphereR; }
    IntegratingSphere<T> getSphereT() const noexcept { return SphereT; }
    DetectorDistance<T> getDist() const noexcept { return dist; }
    std::vector<std::pair<T,T>> getRmeas() const noexcept { return rmeas; }
    std::vector<std::pair<T,T>> getTmeas() const noexcept { return tmeas; }
    T getTcmeas() const noexcept { return tcmeas; }
    T getTau()    const noexcept { return tau;    }
    T getG()      const noexcept { return g;      }

protected:
    T tau;
    T g;

    // Sample<T> sample;
    Medium<T> emptyTissue;
    std::vector<Medium<T>> slides;
    int Np, threads;
    T z;
    T r;
    IntegratingSphere<T> SphereR, SphereT;
    DetectorDistance<T> dist;
    std::vector<std::pair<T,T>> rmeas;
    std::vector<std::pair<T,T>> tmeas;
    T tcmeas;
};

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, Inverse_NS::FixedParameter fix >
T fixParam(T newG, Medium<T> emptyTissue, std::vector<Medium<T>> slides, T tcmeas) {
    using namespace Inverse_NS;
    using namespace std;

    T nSlab = emptyTissue.n;
    T nSlideTop, nSlideBottom;
    if (slides.empty()) {
        nSlideTop = nSlab;
        nSlideBottom = nSlab;
    } else {
        nSlideTop = slides[0].n;
        nSlideBottom = slides[1].n;
    }
    if (fix == FixedParameter::Tau)
        return tauCalc<T>(nSlab, nSlideTop, nSlideBottom, tcmeas);
    else if (fix == FixedParameter::G)
        return newG;
    else
        throw invalid_argument("Need to have fixed parameter");
}

template <typename T, size_t Nz, size_t Nr, bool detector, size_t N, Inverse_NS::FixedParameter fix, size_t gSize>
Matrix<T,gSize,gSize> distancesIMC(FuncMC<T,Nz,Nr,detector,N,fix> f, Matrix<T,1,gSize> gridA, Matrix<T,1,gSize> gridT, Matrix<T,1,gSize> gridG, const T& g) {
    using namespace std;

    Matrix<T,gSize,gSize> dist;
    constexpr T EPS = 1E-6;
    /// TODO: WHAT IS THIS 1E-6?
    T ts0 = 0;
    T rs0 = 0;
    cout << f.getRmeas()[0].second << " " << f.getTmeas()[0].second << endl;
    for (size_t i = 0; i < gSize; i++) {
        for (size_t j = 0; j < gSize; j++) {
            vector<Medium<T>> layers;
            if (f.getSlides().empty())
                layers = { Medium<T>::fromAlbedo(f.getEmptyTissue().n, gridA(i), gridT(j), f.getEmptyTissue().D, g) };
            else
                layers = { f.getSlides()[0], Medium<T>::fromAlbedo(f.getEmptyTissue().n, gridA(i), gridT(j), f.getEmptyTissue().D, g), f.getSlides()[1] };
            Sample<T> sample(layers, 1.0, 1.0);
            MCresults<T,Nz,Nr,detector> myResultsMT;
            DetectorDistance<T> zero_dist;
            MCmultithread<T,Nz,Nr,detector>(sample, 0.5e5, f.getThreads(), f.getZ(), f.getR(), myResultsMT, f.getSphereR(), f.getSphereT(), zero_dist);
            const auto rMC = myResultsMT.detectedR;
            const auto tMC = myResultsMT.detectedT;
            rs0 = rMC[0].second;
            ts0 = tMC[0].second;
            dist(i,j) = abs(rs0 - (f.getRmeas()[0].second)) / ((f.getRmeas()[0].second) + EPS) + abs(ts0 - (f.getTmeas()[0].second)) / ((f.getTmeas()[0].second) + EPS);
            cout << gridA(i) << " " << gridT(j) << " " << g << ": " << rs0 << " " << ts0 << " dist " << dist(i,j) << endl;
            //dist(i,j) = std::abs(rs0 - f.getRmeas()) + std::abs(ts0 - f.getTmeas());
        }
      }
    return dist;
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, Inverse_NS::FixedParameter fix >
void startGridIMC(FuncMC<T,Nz,Nr,detector,N,fix> f, T& aStart, T& tStart, T& gStart) {
    using namespace std;
    constexpr int gridSize = 7;
    Matrix<T, 1, gridSize> gridA, gridT, gridG;
    ConstructGrid<T,gridSize>(gridA, gridT, gridG);

    vector<T> vectorMins;
    vector<pair<int,int>> vectorCoord;
    for (int i = 0; i < gridSize; i++) {
        const auto distancesMatrix = distancesIMC<T, Nz, Nr, detector, N, fix, gridSize>(f, gridA, gridT, gridG, gridG(i));
        int minRow, minCol;
        const T mins = distancesMatrix.minCoeff(&minRow, &minCol);
        const T minHere = distancesMatrix.minCoeff();
        vectorMins.push_back(minHere);
        vectorCoord.push_back({minRow, minCol});
        ignore = mins;
        cout << gridG(i) << ": " << minHere << " " << minRow << " " << minCol << endl;
    }

    int minElementIndex = min_element(vectorMins.begin(),vectorMins.end()) - vectorMins.begin();
    aStart = gridA(vectorCoord[minElementIndex].first);
    tStart = gridT(vectorCoord[minElementIndex].second);
    gStart = gridG(minElementIndex);
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, Inverse_NS::FixedParameter fix >
void NelderMeadMin(FuncMC<T,Nz,Nr,detector,N,fix> f, int maxIter, T astart, T tstart, T gstart, Matrix<T,1,N>& vecMin, T& fmin, int& iters, const T& checkConvEps) {
    using namespace Inverse_NS;
    using namespace Inverse_NS;
    using namespace std;

    Matrix<T,1,N> vstart, vb, vg, vw, vmid, vr, ve, vc, vs, vprevious;
    T alpha = 1.0;
    T beta = 0.5;
    T gamma = 2.0;

    /// INITIALIZING STARTING SIMPLEX

    /*
    if (N == 3)
        vstart << a2aComp<T>(astart), tau2tauComp<T>(tstart), g2gComp<T>(gstart);
    else if (N == 2) {
        if (fix)
            vstart << a2aComp<T>(astart), g2gComp<T>(gstart);
        else
            vstart << a2aComp<T>(astart), tau2tauComp<T>(tstart);
    }
    //*/

    if (astart > 0.99)
        astart -= 0.01;
    if (gstart > 0.99)
        gstart -= 0.01;
    if (gstart < -0.99)
        gstart += 0.01;

    if (N == 3)
        vstart << astart, tstart, gstart;
    else if (N == 2) {
        if (fix == FixedParameter::Tau)
            vstart << astart, gstart;
        else if (fix == FixedParameter::G)
            vstart << astart, tstart;
        else
            throw invalid_argument("Need to have fixed parameter");
    }

    array<Matrix<T,1,N>, N> basis;
    array<Matrix<T,1,N>, N+1> start;
    /// TODO: Don't use pairs, use structs insted
    array<pair<Matrix<T,1,N>,T>,N+1> simplex;
    for (size_t i = 0; i < N; i++) {
        basis[i] = Matrix<T,1,N>::Zero();
        basis[i](i) = 1.0;
    }

    start[0] = vstart;
    simplex[0].first = v2vComp<T,N,fix>(start[0]);
    for (size_t i = 1; i < N + 1; i++) {
        T h = 0;
        if (vstart(i-1) == 0)
            h = 0.0025;
        else if (N == 2 && fix == FixedParameter::Tau) {
            if (vstart(0) <= 0.05) {
                if (abs(vstart(1)) >= 0.95)
                    h = -0.05 * pow(-1, i);
                else
                    h = +0.05;
            } else if (vstart(0) >= 0.95) {
                  if (abs(vstart(1)) <= 0.05)
                      h = +0.05 * pow(-1, i);
                  else
                      h = -0.05;
            } else if (vstart(0) < 0.95 && vstart(0) > 0.05) {
                if (abs(vstart(1)) >= 0.95)
                    h = -0.05;
                else
                    h = +0.05;
            }

            /*
            if (vstart(0) >= 0.95 && vstart(1) >= 0.95)
                h = -0.05;
            else if (vstart(0) <= 0.05 && vstart(1) >= 0.95)
                h = -0.05 * pow(-1, i);
            else if (vstart(0) >= 0.95 && vstart(1) <= 0.05)
                h = 0.05 * pow(-1, i);
            else if (vstart(0) >= 0.95 && vstart(0) <= 0.95)
                h = -0.05;
            else if (vstart(0) <= 0.95 && vstart(0) >= 0.95)
                h = -0.05;
            else
                h = 0.05;
            //*/
        }
        else
            h = 0.05;
        start[i] = start[i - 1] + h * basis[i - 1];
        simplex[i].first = v2vComp<T,N,fix>(start[i]);
    }

    // if (N == 2 & vstart(1) > 0.95)

    vprevious = vComp2v<T,N,fix>(simplex[0].first);

    cout << vComp2v<T,N,fix>(simplex[0].first) << " " << simplex[0].second << endl;
    cout << vComp2v<T,N,fix>(simplex[1].first) << " " << simplex[1].second << endl;
    cout << vComp2v<T,N,fix>(simplex[2].first) << " " << simplex[2].second << endl;

    for (int k = 0; k < maxIter; k++) {
        iters = k;
        T EPS = checkConvEps;
        cout << k << endl;
        /// FIND BEST, GOOD AND WORST VERTICES OF SIMPLEX
        for (size_t i = 0; i < N + 1; i++) {
            simplex[i].second = f.funcToMinimize3argsMC(vComp2v<T,N,fix>(simplex[i].first));
            // cout << simplex[i].first << " " << simplex[i].second << endl;
        }

        sort(ALL_CONTAINER(simplex), SortSimplex<T,N>);
        vb = simplex[0].first;
        vg = simplex[1].first;
        vw = simplex[N].first;

        cout << vComp2v<T,N,fix>(simplex[0].first) << " " << simplex[0].second << endl;
        cout << vComp2v<T,N,fix>(simplex[1].first) << " " << simplex[1].second << endl;
        cout << vComp2v<T,N,fix>(simplex[2].first) << " " << simplex[2].second << endl;

        /// FIND CENTER OF MASS OF EVERYTHING EXCEPT WORST VERTEX
        vmid = Matrix<T,1,N>::Zero();
        for (size_t i = 0; i < N; i++) {
            T sum = 0;
            for (size_t j = 0; j < N; j++)
                sum += simplex[j].first(i);
            sum /= N;
            vmid(i) = sum;
        }
        /// REFLECTION
        vr = (1 + alpha) * vmid - alpha*vw;
        /// IS VR A GOOD VERTEX?
        T fvr = f.funcToMinimize3argsMC(vComp2v<T,N,fix>(vr));
        T fvb = simplex[0].second;
        T fvg = simplex[1].second;
        T fvw = simplex[2].second;
        /*
        cout << "Vmid " << vmid << " " << vComp2v<T,N,fix>(vmid) << endl;
        cout << "VR " << vr << " " << vComp2v<T,N,fix>(vr) << " " << fvr << endl;
        //*/
        if (fvr < fvb) {
            /// EXPANSION
            ve = (1 - gamma)*vmid + gamma*vr;
            T fve = f.funcToMinimize3argsMC(vComp2v<T,N,fix>(ve));
            // cout << "VE " << ve << " " << vComp2v<T,N,fix>(ve) << " " << fve << endl;
            if (fve < fvr) {
                simplex[N].first = ve;
                simplex[N].second = fve;

                int checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                cout << "v previous " << vprevious << endl;
                // cout << "expanded" << endl;
                continue;
            } else {
                simplex[N].first = vr;
                simplex[N].second = fvr;

                int checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                cout << "v previous " << vprevious << endl;
                // cout << "1" << endl;
                continue;
            }
        } else if (fvb < fvr && fvr < fvg) {
            simplex[N].first = vr;
            simplex[N].second = fvr;

            int checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            cout << "v previous " << vprevious << endl;
            // cout << "2" << endl;
            continue;
        } else if (fvg < fvr && fvr < fvw) {
            Matrix<T,1,N> cache = vr;
            vr = vw;
            vw = cache;
        }
        /// SHRINK
        vs = v2vComp<T,N,fix>(beta * vComp2v<T,N,fix>(vw) + (1 - beta) * vComp2v<T,N,fix>(vmid));
        T fvs = f.funcToMinimize3argsMC(vComp2v<T,N,fix>(vs));
        // T fve = f.funcToMinimize3argsMC(vComp2v<T,N,fix>(ve));
        /*
        cout << "VS " << vs << " " << vComp2v<T,N,fix>(vs) << " " << fvs << endl;
        cout << vComp2v<T,N,fix>(vs) << " " << fvs << endl;
        //*/
        if (fvs < f.funcToMinimize3argsMC(vComp2v<T,N,fix>(vw))) {
            simplex[N].first = vs;
            simplex[N].second = fvs;

        int checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            cout << "v previous " << vprevious << endl;
            // cout << "shrink" << endl;
            continue;
        } else {
            /// GLOBAL SHRINK;
            for (size_t i = 1; i <= N; i++) {
                simplex[i].first = simplex[0].first + (simplex[i].first - simplex[0].first) / 2;
                simplex[i].second = f.funcToMinimize3argsMC(vComp2v<T,N,fix>(simplex[i].first));
            }
            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            cout << "v previous " << vprevious << endl;
            // cout << "global shrink" << endl;

            int checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
            if (checksum == N)
                break;

            continue;
        }
    }

    sort(ALL_CONTAINER(simplex), SortSimplex<T,N>);
    // cout << "MINIMUM " << simplex[0].second << " AT POINT " << simplex[0].first << endl;
    vecMin = vComp2v<T,N,fix>(simplex[0].first);
    fmin = simplex[0].second;
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, Inverse_NS::FixedParameter fix >
void IMC(const std::vector<std::pair<T,T>>& rmeas,
         const std::vector<std::pair<T,T>>& tmeas,
         T tcmeas,
         const Medium<T>& emptyTissue,
         const std::vector<Medium<T>>& slides,
         int Np,
         int threads,
         T z,
         T r,
         const IntegratingSphere<T>& SphereR,
         const IntegratingSphere<T>& SphereT,
         const DetectorDistance<T>& dist,
         const T& aStart,
         const T& tStart,
         const T& gStart,
         const T& checkConvEps,
         T& aOut,
         T& tauOut,
         T& gOut) {
    using namespace Inverse_NS;
	using namespace std;

    T fixedParam = fixParam<T,Nz,Nr,detector,N,fix>(gStart, emptyTissue, slides, tcmeas);// fix == 1 => any arg, fix == 0 => value of g
    FuncMC<T,Nz,Nr,detector,N,fix> toMinimize(fixedParam, emptyTissue, slides, Np, threads, z, r, SphereR, SphereT, dist, rmeas, tmeas, tcmeas);

    if (fix == FixedParameter::Tau && N == 2)
        cout << "Inverse Monte Carlo, fixed optical thickness = " << tStart << endl;
    else if (fix == FixedParameter::G && N == 2)
        cout << "Inverse Monte Carlo, fixed anisotropy = " << gStart << endl;
    else if (N == 3)
        cout << "Inverse Monte Carlo N = 3" << endl;
    else
        throw invalid_argument("Need to have fixed parameter");

    // cout << astart << " " << gstart << endl;

    int maxIter = 50;
    if (N == 3)
        maxIter = 100;

    T fmin;
    Matrix<T,1,N> vecMin;

    int itersMade;

    NelderMeadMin<T,Nz,Nr,detector,N,fix>(toMinimize, maxIter, aStart, tStart, gStart, vecMin, fmin, itersMade, checkConvEps);

    cout << "Iterations made " << itersMade << endl;

    /*
    if (itersMade == maxIter - 1) { //RESTART
        cout << "Restart" << endl;
        if (fix) {
            aStart = vecMin(0)+0.05;
            gStart = vecMin(1)-0.05;
        } else {
            aStart = vecMin(0)+0.05;
            tStart = vecMin(1)+1;
        }
        NelderMeadMin<T,Nz,Nr,detector,N,fix>(toMinimize, maxIter, aStart, tStart, gStart, vecMin, fmin, itersMade);
        cout << "Iterations made " << itersMade << endl;
    }
    //*/

    if (fix == FixedParameter::Tau && N == 2) {
        cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", g = " << vecMin(1) << ", tau = " << fixedParam << endl;
        aOut = vecMin(0);
        tauOut = fixedParam;
        gOut = vecMin(1);
    } else if (fix == FixedParameter::G && N == 2) {
        cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << ", g = " << fixedParam << endl;
        aOut = vecMin(0);
        tauOut = vecMin(1);
        gOut = fixedParam;
    } else if (N == 3) {
        cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << ", g = " << vecMin(2) << endl;
        aOut = vecMin(0);
        tauOut = vecMin(1);
        gOut = vecMin(2);
    } else
        throw invalid_argument("Need to have fixed parameter");
}
