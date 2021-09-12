#pragma once

#include "MCmultithread.h"
#include "MonteCarlo.h"

#include "../AD/NelderMead.h"
#include "../Physics/Angles.h"
#include "../Utils/Utils.h"

#include <stdexcept>
#include <utility>

template < typename T >
T tauCalc(T nSlab, T n_slide_top, T n_slide_bottom, T Tcol) {
    const auto rb1 = Physics_NS::BorderReflectance<T>(nSlab, n_slide_top   );
    const auto rb2 = Physics_NS::BorderReflectance<T>(nSlab, n_slide_bottom);
    const auto cached1 = rb1 * rb2;
    const auto cached2 = cached1 - rb1 - rb2 + 1;
    return log((sqrt(4 * cached1 * Math_NS::sqr(Tcol) + Math_NS::sqr(cached2)) + cached2) / (2 * Tcol));
}

template < typename T, size_t Nz, size_t Nr, bool detector >
T funcToMinimizeMC(const T& a,
                   const T& tau,
                   const T& g,
                   const Medium<T>& empty_tissue,
                   const std::vector<Medium<T>>& slides,
                   int Np,
                   int threads,
                   T z,
                   T r,
                   const IntegratingSphere<T>& new_sphereR,
                   const IntegratingSphere<T>& new_sphereT,
                   const DetectorDistance<T> new_dist,
                   const std::vector<std::pair<T,T>>& rmeas,
                   const std::vector<std::pair<T,T>>& tmeas) {
    // auto tissue = Medium<T>::fromAlbedo(empty_tissue.n, a, tau, empty_tissue.D, g);
    std::vector<Medium<T>> layers;
    if (slides.empty())
        layers = { Medium<T>::fromAlbedo(empty_tissue.n, a, tau, empty_tissue.D, g) };
    else
        layers = { slides[0], Medium<T>::fromAlbedo(empty_tissue.n, a, tau, empty_tissue.D, g), slides[1] };
    Sample<T> sample(layers, 1.0, 1.0);

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(sample, Np, threads, z, r, myResultsMT, new_sphereR, new_sphereT, new_dist);

    const auto rMC = myResultsMT.detectedR;
    const auto tMC = myResultsMT.detectedT;
    T func2min = 0;

    constexpr auto eps = 1E-6;
    for (int i = 0; i < Utils_NS::isize(rMC); i++)
        func2min += fabs((rMC[i].second - rmeas[i].second) / (rmeas[i].second + eps)) + fabs((tMC[i].second - tmeas[i].second) / (tmeas[i].second + eps));

    return func2min;
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix >
class MinimizableMC {
public:
    virtual T funcToMinimize3argsMC(Matrix<T,1,N> vec) const = 0;

    virtual ~MinimizableMC() = default;
};

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix >
class funcMC: public MinimizableMC<T,Nz,Nr,detector,N,fix> {
public:
    funcMC(T fixed_param, const Medium<T>& new_empty_tissue, const std::vector<Medium<T>>& new_slides, int new_Np,
           int new_threads, T new_z, T new_r, const IntegratingSphere<T>& new_sphereR, const IntegratingSphere<T>& new_sphereT,
           const DetectorDistance<T> new_dist, const std::vector<std::pair<T,T>>& new_rmeas, const std::vector<std::pair<T,T>>& new_tmeas, const T& new_tcmeas) noexcept
        : empty_tissue(new_empty_tissue)
        , slides(new_slides)
        , Np(new_Np)
        , threads(new_threads)
        , z(new_z), r(new_r)
        , SphereR(new_sphereR)
        , SphereT(new_sphereT)
        , dist(new_dist)
        , rmeas(new_rmeas)
        , tmeas(new_tmeas)
        , tcmeas(new_tcmeas) {
        if (fix)
            this->tau = fixed_param;
        else
            this->g = fixed_param;
    }

    T funcToMinimize3argsMC(Matrix<T,1,N> vec) const {
        if (N == 2) {
            if (fix)
                return funcToMinimizeMC<T,Nz,Nr,detector>(vec(0), this->tau, vec(1), this->empty_tissue, this->slides, this->Np, this->threads, this->z, this->r, this->SphereR, this->SphereT, this->dist, this->rmeas, this->tmeas);
            else
                return funcToMinimizeMC<T,Nz,Nr,detector>(vec(0), vec(1), this->g, this->empty_tissue, this->slides, this->Np, this->threads, this->z, this->r, this->SphereR, this->SphereT, this->dist, this->rmeas, this->tmeas);
        } else if (N == 3)
            return funcToMinimizeMC<T,Nz,Nr,detector>(vec(0), vec(1), vec(2), this->empty_tissue, this->slides, this->Np, this->threads, this->z, this->r, this->SphereR, this->SphereT, this->dist, this->rmeas, this->tmeas);
        else
            throw std::invalid_argument("N should be in range [2, 3]");
    }

    Medium<T> getEmptyTissue() const noexcept { return empty_tissue; }
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
    T getTau() const noexcept { return tau; }
    T getG() const noexcept { return g; }

protected:
    T tau;
    T g;

    // Sample<T> sample;
    Medium<T> empty_tissue;
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

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix >
T fixParam(T newG, Medium<T> empty_tissue, std::vector<Medium<T>> slides, T tcmeas) {
    T nSlab = empty_tissue.n;
    T n_slide_top, n_slide_bottom;
    if (slides.empty()) {
        n_slide_top = nSlab;
        n_slide_bottom = nSlab;
    } else {
        n_slide_top = slides[0].n;
        n_slide_bottom = slides[1].n;
    }
    if (fix) // fixed tau
        return tauCalc<T>(nSlab, n_slide_top, n_slide_bottom, tcmeas); // tau
    else // fixed g
        return newG; // g
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix >
void NelderMeadMin(funcMC<T,Nz,Nr,detector,N,fix> f, int maxIter, T astart, T tstart, T gstart, Matrix<T,1,N>& vecMin, T& fmin, int& iters, const T& checkConvEps) {
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
        if (fix)
            vstart << astart, gstart;
        else
            vstart << astart, tstart;
    }

    std::array<Matrix<T,1,N>, N> basis;
    std::array<Matrix<T,1,N>, N+1> start;
    /// TODO: Don't use pairs, use structs insted
    std::array<std::pair<Matrix<T,1,N>,T>,N+1> simplex;
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
        else if (N == 2 && fix) {
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

    /*
    simplex[0].first << 0.866951, 0.903689;
    simplex[1].first << 0.849763, 0.906033;
    simplex[2].first << 0.857088, 0.902761;
    //*/

    std::cout << vComp2v<T,N,fix>(simplex[0].first) << " " << simplex[0].second << std::endl;
    std::cout << vComp2v<T,N,fix>(simplex[1].first) << " " << simplex[1].second << std::endl;
    std::cout << vComp2v<T,N,fix>(simplex[2].first) << " " << simplex[2].second << std::endl;

    for (int k = 0; k < maxIter; k++) {
        iters = k;
        T eps = checkConvEps;
        std::cout << k << std::endl;

        /// FIND BEST, GOOD AND WORST VERTICES OF SIMPLEX
        for (size_t i = 0; i < N + 1; i++) {
            simplex[i].second = f.funcToMinimize3argsMC(vComp2v<T,N,fix>(simplex[i].first));
            // cout << simplex[i].first << " " << simplex[i].second << endl;
        }

        std::sort(ALL_CONTAINER(simplex), sortSimplex<T, N>);
        vb = simplex[0].first;
        vg = simplex[1].first;
        vw = simplex[N].first;

        std::cout << simplex[0].first << " " << vComp2v<T,N,fix>(simplex[0].first) << " " << simplex[0].second << std::endl;
        std::cout << simplex[1].first << " " << vComp2v<T,N,fix>(simplex[1].first) << " " << simplex[1].second << std::endl;
        std::cout << simplex[2].first << " " << vComp2v<T,N,fix>(simplex[2].first) << " " << simplex[2].second << std::endl;

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

                int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                std::cout << "v previous " << vprevious << std::endl;
                // cout << "expanded" << endl;
                continue;
            } else {
                simplex[N].first = vr;
                simplex[N].second = fvr;

                int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                std::cout << "v previous " << vprevious << std::endl;
                // cout << "1" << endl;
                continue;
            }
        } else if (fvb < fvr && fvr < fvg) {
            simplex[N].first = vr;
            simplex[N].second = fvr;

            int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            std::cout << "v previous " << vprevious << std::endl;
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

        int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            std::cout << "v previous " << vprevious << std::endl;
            // cout << "shrink" << endl;
            continue;
        } else {
            /// GLOBAL SHRINK;
            for (size_t i = 1; i <= N; i++) {
                simplex[i].first = simplex[0].first + (simplex[i].first - simplex[0].first) / 2;
                simplex[i].second = f.funcToMinimize3argsMC(vComp2v<T,N,fix>(simplex[i].first));
            }
            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            std::cout << "v previous " << vprevious << std::endl;
            // cout << "global shrink" << endl;

            int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
            if (checksum == N)
                break;

            continue;
        }
    }

    std::sort(ALL_CONTAINER(simplex), sortSimplex<T, N>);
    // cout << "MINIMUM " << simplex[0].second << " AT POINT " << simplex[0].first << endl;
    vecMin = vComp2v<T,N,fix>(simplex[0].first);
    fmin = simplex[0].second;
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix >
void IMC(const std::vector<std::pair<T,T>>& rmeas,
         const std::vector<std::pair<T,T>>& tmeas,
         T tcmeas,
         const Medium<T>& empty_tissue,
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
    T fixedParam = fixParam<T,Nz,Nr,detector,N,fix>(0.0, empty_tissue, slides, tcmeas);// fix == 1 => any arg, fix == 0 => value of g
    funcMC<T,Nz,Nr,detector,N,fix> toMinimize(fixedParam, empty_tissue, slides, Np, threads, z, r, SphereR, SphereT, dist, rmeas, tmeas, tcmeas);


    if (fix)
        std::cout << "Inverse Monte Carlo, fixed optical thickness = " << tStart << std::endl;
    else
        std::cout << "Inverse Monte Carlo, fixed anisotropy = " << gStart << std::endl;

    // cout << astart << " " << gstart << endl;

    int maxIter = 50;

    T fmin;
    Matrix<T,1,N> vecMin;

    int itersMade;

    NelderMeadMin<T,Nz,Nr,detector,N,fix>(toMinimize, maxIter, aStart, tStart, gStart, vecMin, fmin, itersMade, checkConvEps);

    std::cout << "Iterations made " << itersMade << std::endl;

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

    if (fix) {
        std::cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", g = " << vecMin(1) << ", tau = " << fixedParam << std::endl;
        aOut = vecMin(0);
        tauOut = fixedParam;
        gOut = vecMin(1);
    } else {
        std::cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << ", g = " << fixedParam << std::endl;
        aOut = vecMin(0);
        tauOut = vecMin(1);
        gOut = fixedParam;
    }
}
