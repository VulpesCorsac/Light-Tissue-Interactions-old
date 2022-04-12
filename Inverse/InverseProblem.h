#pragma once

#include "../AD/Quadrature.h"
#include "../eigen/Eigen/Dense"
#include "../AD/RT.h"
#include "NelderMead.h"
#include "../MC/MonteCarlo.h"
#include "../MC/MonteCarloMultithread.h"

using namespace Inverse_NS;

template < typename T >
T tauCalc(T nSlab, T nSlideTop, T nSlideBottom, T Tcol) {
    using namespace Math_NS;
    using namespace Physics_NS;

    const auto rb1 = BorderReflectance<T>(nSlab, nSlideTop   );
    const auto rb2 = BorderReflectance<T>(nSlab, nSlideBottom);
    const auto cached1 = rb1 * rb2;
    const auto cached2 = cached1 - rb1 - rb2 + 1;

    CHECK_ARGUMENT_CONTRACT(Tcol != 0);
    CHECK_RUNTIME_CONTRACT(4 * cache1 * sqr(Tcol) + sqr(cache2) > 0);
    CHECK_RUNTIME_CONTRACT((sqrt(4 * cache1 * sqr(Tcol) + sqr(cache2)) + cache2) / (2 * Tcol) > 0);

    return log((sqrt(4 * cached1 * sqr(Tcol) + sqr(cached2)) + cached2) / (2 * Tcol));
}

template < typename T, Inverse_NS::FixedParameter fix >
T fixParam(T newG, const Sample<T>& emptySample, T tcmeas) {
    using namespace Inverse_NS;
    T nSlab = emptySample.getNslab();
    T nSlideTop = emptySample.getNslideTop();
    T nSlideBottom = emptySample.getNslideBottom();

    CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

    if (fix == FixedParameter::Tau)
        return tauCalc<T>(nSlab, nSlideTop, nSlideBottom, tcmeas);
    else if (fix == FixedParameter::G)
        return newG;
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t M, size_t N, Inverse_NS::FixedParameter fix >
class Minimizable {
public:
    virtual T MinimizationMC3args(Matrix<T,1,N> vec) = 0;
    virtual T MinimizationAD3args(Matrix<T,1,N> vec) = 0;
    virtual ~Minimizable() = default;
};

template < typename T, size_t Nz, size_t Nr, bool detector, size_t M, size_t N, Inverse_NS::FixedParameter fix >
class Func : public Minimizable<T,Nz,Nr,detector,M,N,fix> {
public:
    Func(T fixedParam, const Sample<T>& emptySampleNew, const T& rmeasNew, const T& tmeasNew, const T& tcmeasNew) EXCEPT_INPUT_PARAMS
        : emptySample(emptySampleNew)
        , tcmeas(tcmeasNew) {
        using namespace Inverse_NS;
        using namespace std;

        if (emptySampleNew.getNlayers() == 3) {
            this->glassTop    = Medium<T>::fromAlbedo(emptySampleNew.getNslideTop(),    0.0, 0.0, emptySample.getMedium(0).getD(), 0.0);
            this->glassBottom = Medium<T>::fromAlbedo(emptySampleNew.getNslideBottom(), 0.0, 0.0, emptySample.getMedium(2).getD(), 0.0);
        }

        this->nLayers = emptySampleNew.getNlayers();

        this->rmeas.push_back(make_pair(0.0, rmeasNew));
        this->tmeas.push_back(make_pair(0.0, tmeasNew));

        CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

        if (fix == FixedParameter::Tau)
            this->tau = fixedParam;
        else if (fix == FixedParameter::G)
            this->g = fixedParam;
        else
            throw invalid_argument("Need to have fixed parameter");
    }

    Func(T fixedParam, const Sample<T>& emptySampleNew, const int& NpNew,
         const int& threadsNew, const T& zNew, const T& rNew, const IntegratingSphere<T>& sphereRNew, const IntegratingSphere<T>& sphereTNew,
         const DetectorDistance<T>& distNew, const std::vector<std::pair<T,T>>& rmeasNew, const std::vector<std::pair<T,T>>& tmeasNew, const T& tcmeasNew) EXCEPT_INPUT_PARAMS
        : emptySample(emptySampleNew)
        , nLayers(emptySampleNew.getNlayers())
        , Np(NpNew)
        , threads(threadsNew)
        , z(zNew)
        , r(rNew)
        , SphereR(sphereRNew)
        , SphereT(sphereTNew)
        , dist(distNew)
        , rmeas(rmeasNew)
        , tmeas(tmeasNew)
        , tcmeas(tcmeasNew) {
        using namespace Inverse_NS;
        using namespace std;

        if (emptySampleNew.getNlayers() == 3) {
            this->glassTop    = Medium<T>::fromAlbedo(emptySample.getNslideTop(),    0.0, 0.0, emptySample.getMedium(0).getD(), 0.0);
            this->glassBottom = Medium<T>::fromAlbedo(emptySample.getNslideBottom(), 0.0, 0.0, emptySample.getMedium(2).getD(), 0.0);
        }

        this->nLayers = emptySampleNew.getNlayers();

        CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

        if (fix == FixedParameter::Tau)
            this->tau = fixedParam;
        else if (fix == FixedParameter::G)
            this->g = fixedParam;
        else
            throw invalid_argument("Need to have fixed parameter");
    }

    T MinimizationAD(const Sample<T>& sample) {
        using namespace Utils_NS;
        using namespace AddingDoubling_NS;
        using namespace std;
        Quadrature<T,M> quadrature(sample.getNslab());
        const auto v = quadrature.getV();
        const auto w = quadrature.getW();
        T ts, rs;
        RTs<T,M>(sample, v, w, rs, ts);
        constexpr T EPS = 1E-6;
        CHECK_ARGUMENT_CONTRACT(this->rmeas[0].second + EPS > 0);
        CHECK_ARGUMENT_CONTRACT(this->tmeas[0].second + EPS > 0);

        return abs((rs - this->rmeas[0].second) / (this->rmeas[0].second + EPS)) + abs((ts - this->tmeas[0].second) / (this->tmeas[0].second + EPS));
    }

    T MinimizationMC (const Sample<T>& sample) {
        using namespace Utils_NS;
        using namespace std;

        MCresults<T,Nz,Nr,detector> myResultsMT;
        MCmultithread<T,Nz,Nr,detector>(sample, this->Np, this->threads, this->z, this->r, myResultsMT, this->SphereR, this->SphereT, this->dist);
        const auto rMC = myResultsMT.detectedR;
        const auto tMC = myResultsMT.detectedT;
        T func2min = 0;

        constexpr auto EPS = 1E-6;
        for (int i = 0; i < isize(rMC); i++)
            func2min += abs((rMC[i].second - this->rmeas[i].second) / (this->rmeas[i].second + EPS)) + abs((tMC[i].second - this->tmeas[i].second) / (this->tmeas[i].second + EPS));

        return func2min;
    }

    T MinimizationAD3args(Matrix<T,1,N> vec) {
        using namespace std;
        using namespace Inverse_NS;
        vector<Medium<T>> sample;
        CHECK_ARGUMENT_CONTRACT(this->emptySample.getNlayers() == 3 || this->emptySample.getNlayers() == 1);
        CHECK_ARGUMENT_CONTRACT(2 <= N && N <= 3);
        if (N == 2) {
            CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);
            if (fix == FixedParameter::Tau) {
                if (this->nLayers == 3)
                    sample = {this->glassTop, Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), this->tau, emptySample.getMedium(1).getD(), vec(1)), this->glassBottom};
                else if (this->nLayers == 1)
                    sample = {Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), this->tau, emptySample.getMedium(0).getD(), vec(1))};
                return MinimizationAD(Sample<T>(sample));
            }
            else if (fix == FixedParameter::G) {
                if (this->nLayers == 3)
                    sample = {this->glassTop, Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), vec(1), emptySample.getMedium(1).getD(), this->g), this->glassBottom};
                else if (this->nLayers == 1)
                    sample = {Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), vec(1), emptySample.getMedium(0).getD(), this->g)};
                return MinimizationAD(Sample<T>(sample));
            }
        } else if (N == 3) {
            if (this->nLayers == 3)
                sample = {this->glassTop, Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), vec(1), emptySample.getMedium(1).getD(), vec(2)), this->glassBottom};
            else if (this->nLayers == 1)
                sample = {Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), vec(1), emptySample.getMedium(0).getD(), vec(2))};
            return MinimizationAD(Sample<T>(sample));
        }
    }

    T MinimizationMC3args(Matrix<T,1,N> vec) {
        using namespace std;
        using namespace Inverse_NS;
        vector<Medium<T>> sample;
        CHECK_ARGUMENT_CONTRACT(this->emptySample.getNlayers() == 3 || this->emptySample.getNlayers() == 1);
        CHECK_ARGUMENT_CONTRACT(2 <= N && N <= 3);
        if (N == 2) {
            CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);
            if (fix == FixedParameter::Tau) {
                if (this->nLayers == 3)
                    sample = {this->glassTop, Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), this->tau, emptySample.getMedium(1).getD(), vec(1)), this->glassBottom};
                else if (this->nLayers == 1)
                    sample = {Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), this->tau, emptySample.getMedium(0).getD(), vec(1))};
                return MinimizationMC(Sample<T>(sample));
            }
            else if (fix == FixedParameter::G) {
                if (this->nLayers == 3)
                    sample = {this->glassTop, Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), vec(1), emptySample.getMedium(1).getD(), this->g), this->glassBottom};
                else if (this->nLayers == 1)
                    sample = {Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), vec(1), emptySample.getMedium(0).getD(), this->g)};
                return MinimizationMC(Sample<T>(sample));
            }
        } else if (N == 3) {
            if (this->nLayers == 3)
                sample = {this->glassTop, Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), vec(1), emptySample.getMedium(1).getD(), vec(2)), this->glassBottom};
            else if (this->nLayers == 1)
                sample = {Medium<T>::fromAlbedo(emptySample.getNslab(), vec(0), vec(1), emptySample.getMedium(0).getD(), vec(2))};
            return MinimizationMC(Sample<T>(sample));
        }
    }

    void NelderMeadMin(int maxIter, T astart, T tstart, T gstart,
                   Matrix<T,1,N>& vecMin, T& fmin, int& iters, const T& checkConvEps, const Inverse_NS::ModellingMethod& mod) {
        using namespace std;
        using namespace Inverse_NS;

        CHECK_ARGUMENT_CONTRACT(mod == ModellingMethod::MC || mod == ModellingMethod::AD);

        bool debug = 0;

        Matrix<T,1,N> vstart, vb, vg, vw, vmid, vr, ve, vc, vs, vprevious;
        T alpha = 1.0;
        T beta = 0.5;
        T gamma = 2.0;

        /// INITIALIZING STARTING SIMPLEX

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

        vprevious = vComp2v<T,N,fix>(simplex[0].first);

        cerr << vComp2v<T,N,fix>(simplex[0].first) << " " << simplex[0].second << endl;
        cerr << vComp2v<T,N,fix>(simplex[1].first) << " " << simplex[1].second << endl;
        cerr << vComp2v<T,N,fix>(simplex[2].first) << " " << simplex[2].second << endl;

        for (int k = 0; k < maxIter; k++) {
            iters = k;
            T EPS = checkConvEps;
            cerr << "Iteration " << k << endl;
            /// FIND BEST, GOOD AND WORST VERTICES OF SIMPLEX
            for (size_t i = 0; i < N + 1; i++) {
                if (mod == ModellingMethod::AD)
                    simplex[i].second = MinimizationAD3args(vComp2v<T,N,fix>(simplex[i].first));
                else if (mod == ModellingMethod::MC)
                    simplex[i].second = MinimizationMC3args(vComp2v<T,N,fix>(simplex[i].first));
            }

            sort(ALL_CONTAINER(simplex), SortSimplex<T,N>);
            vb = simplex[0].first;
            vg = simplex[1].first;
            vw = simplex[N].first;

            cerr << vComp2v<T,N,fix>(simplex[0].first) << " " << simplex[0].second << endl;
            cerr << vComp2v<T,N,fix>(simplex[1].first) << " " << simplex[1].second << endl;
            cerr << vComp2v<T,N,fix>(simplex[2].first) << " " << simplex[2].second << endl;

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
            T fvr = 0.0;
            if (mod == ModellingMethod::AD)
                fvr = MinimizationAD3args(vComp2v<T,N,fix>(vr));
            else if (mod == ModellingMethod::MC)
                fvr = MinimizationMC3args(vComp2v<T,N,fix>(vr));
            T fvb = simplex[0].second;
            T fvg = simplex[1].second;
            T fvw = simplex[2].second;

            if (debug) {
                cerr << "Vmid " << vmid << " " << vComp2v<T,N,fix>(vmid) << endl;
                cerr << "VR " << vr << " " << vComp2v<T,N,fix>(vr) << " " << fvr << endl;
            }
            //*/
            if (fvr < fvb) {
                if (debug)
                    cerr << "EXPANSION" << endl;
                /// EXPANSION
                ve = (1 - gamma)*vmid + gamma*vr;
                T fve = 0.0;
                if (mod == ModellingMethod::AD)
                    fve = MinimizationAD3args(vComp2v<T,N,fix>(ve));
                else if (mod == ModellingMethod::MC)
                    fve = MinimizationMC3args(vComp2v<T,N,fix>(ve));
                if (debug)
                    cerr << "VE " << ve << " " << vComp2v<T,N,fix>(ve) << " " << fve << endl;
                if (fve < fvr) {
                    simplex[N].first = ve;
                    simplex[N].second = fve;

                    int checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
                    if (checksum == N)
                        break;

                    vprevious = vComp2v<T,N,fix>(simplex[N].first);
                    if (debug)
                        cerr << "v previous " << vprevious << endl;
                    continue;
                } else {
                    simplex[N].first = vr;
                    simplex[N].second = fvr;

                    int checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
                    if (checksum == N)
                        break;

                    vprevious = vComp2v<T,N,fix>(simplex[N].first);
                    if (debug)
                        cerr << "v previous " << vprevious << endl;
                    continue;
                }
            } else if (fvb < fvr && fvr < fvg) {
                simplex[N].first = vr;
                simplex[N].second = fvr;

                int checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                if (debug)
                    cerr << "v previous " << vprevious << endl;
                continue;
            } else if (fvg < fvr && fvr < fvw) {
                Matrix<T,1,N> cache = vr;
                vr = vw;
                vw = cache;
            }
            /// SHRINK
            if (debug)
                cerr << "SHRINK" << endl;
            vs = v2vComp<T,N,fix>(beta * vComp2v<T,N,fix>(vw) + (1 - beta) * vComp2v<T,N,fix>(vmid));
            T fvs = 0.0;
            T fve = 0.0;
            fvw = 0.0;
            if (mod == ModellingMethod::AD) {
                fvs = MinimizationAD3args(vComp2v<T,N,fix>(vs));
                fve = MinimizationAD3args(vComp2v<T,N,fix>(ve));
                fvw = MinimizationAD3args(vComp2v<T,N,fix>(vw));
            }
            else if (mod == ModellingMethod::MC) {
                fvs = MinimizationMC3args(vComp2v<T,N,fix>(vs));
                fve = MinimizationMC3args(vComp2v<T,N,fix>(ve));
                fvw = MinimizationMC3args(vComp2v<T,N,fix>(vw));
            }
            if (debug){
                cerr << "VS " << vs << " " << vComp2v<T,N,fix>(vs) << " " << fvs << endl;
                cerr << vComp2v<T,N,fix>(vs) << " " << fvs << endl;
            }

            if (fvs < fvw) {
                simplex[N].first = vs;
                simplex[N].second = fvs;

            int checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                if (debug)
                    cerr << "v previous " << vprevious << endl;
                continue;
            } else {
                /// GLOBAL SHRINK;
                if (debug)
                    cerr << "GLOBAL SHRINK" << endl;
                for (size_t i = 1; i <= N; i++) {
                    simplex[i].first = simplex[0].first + (simplex[i].first - simplex[0].first) / 2;
                    if (mod == ModellingMethod::AD)
                        simplex[i].second = MinimizationAD3args(vComp2v<T,N,fix>(simplex[i].first));
                    else if (mod == ModellingMethod::MC)
                        simplex[i].second = MinimizationMC3args(vComp2v<T,N,fix>(simplex[i].first));
                }
                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                if (debug)
                    cerr << "v previous " << vprevious << endl;

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

    void InverseProblem (const T& aStart, const T& tStart, const T& gStart, T& aOut, T& tOut, T& gOut, const Inverse_NS::ModellingMethod& mod) {
        using namespace Inverse_NS;
        using namespace std;

        CHECK_ARGUMENT_CONTRACT(mod == ModellingMethod::MC || mod == ModellingMethod::AD);
        CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

        T fixedParam = fixParam<T,fix>(gStart, this->emptySample, this->tcmeas);

        string method = "";
        if (mod == ModellingMethod::AD)
            method = "Adding-doubling";
        else if (mod == ModellingMethod::MC)
            method = "Monte Carlo";

        if (fix == FixedParameter::Tau && N == 2)
            cerr << "Inverse " << method << ", fixed optical thickness = " << tStart << endl;
        else if (fix == FixedParameter::G && N == 2)
            cerr << "Inverse " << method << ", fixed anisotropy = " << gStart << endl;
        else if (N == 3)
            cerr << "Inverse " << method << ", N = 3" << endl;
        else
            throw invalid_argument("Need to have fixed parameter");

        int maxIter = 50;
        if (N == 3)
            maxIter = 100;

        T fmin;
        Matrix<T,1,N> vecMin;
        int itersMade;
        T checkConvEps;

        if (mod == ModellingMethod::MC)
            checkConvEps = 1e-3;
        else if (mod == ModellingMethod::AD)
            checkConvEps = 1e-5;
        NelderMeadMin(maxIter, aStart, tStart, gStart, vecMin, fmin, itersMade, checkConvEps, mod);

        cerr << "Iterations made " << itersMade << endl;

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
            cerr << "Minimum " << fmin << " at point a = " << vecMin(0) << ", g = " << vecMin(1) << ", tau = " << fixedParam << endl;
            aOut = vecMin(0);
            tOut = fixedParam;
            gOut = vecMin(1);
        } else if (fix == FixedParameter::G && N == 2) {
            cerr << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << ", g = " << fixedParam << endl;
            aOut = vecMin(0);
            tOut = vecMin(1);
            gOut = fixedParam;
        } else if (N == 3) {
            cerr << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << ", g = " << vecMin(2) << endl;
            aOut = vecMin(0);
            tOut = vecMin(1);
            gOut = vecMin(2);
        } else
            throw invalid_argument("Need to have fixed parameter");
    }

    Sample<T> getEmptySample()             const noexcept { return emptySample; }
    Medium<T> getGlassTop()                const noexcept { return glassTop;    }
    Medium<T> getGlassBottom()             const noexcept { return glassBottom; }
    IntegratingSphere<T> getSphereR()      const noexcept { return SphereR;     }
    IntegratingSphere<T> getSphereT()      const noexcept { return SphereT;     }
    DetectorDistance<T> getDistances()     const noexcept { return dist;        }
    std::vector<std::pair<T,T>> getRmeas() const noexcept { return rmeas;       }
    std::vector<std::pair<T,T>> getTmeas() const noexcept { return tmeas;       }

    int getNphotons() const noexcept { return Np;      }
    int getNthreads() const noexcept { return threads; }
    T getZ()          const noexcept { return z;       }
    T getR()          const noexcept { return r;       }
    T getTcmeas()     const noexcept { return tcmeas;  }
    T getTau()        const noexcept { return tau;     }
    T getG()          const noexcept { return g;       }

protected:
    Sample<T> emptySample;
    Medium<T> glassTop;
    Medium<T> glassBottom;
    int nLayers;
    int Np, threads;
    T z, r;
    IntegratingSphere<T> SphereR, SphereT;
    DetectorDistance<T> dist;
    std::vector<std::pair<T,T>> rmeas;
    std::vector<std::pair<T,T>> tmeas;
    T tcmeas;
    T tau, g;
};


