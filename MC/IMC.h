#pragma once

#include <utility>
#include <stdexcept>

#include "MonteCarlo.h"
#include "MCmultithread.h"

#include "../AD/NelderMead.h"

template <typename T >
T tauCalc(T n_slab, T n_slide_top, T n_slide_bottom, T Tcol) {
    const auto Rb1 = Rborder<T>(n_slab, n_slide_top);
    const auto Rb2 = Rborder<T>(n_slab, n_slide_bottom);
    return log((sqrt(4 * Rb1 * Rb2 * sqr(Tcol) + sqr(Rb1 * Rb2 - Rb1 - Rb2 + 1)) + Rb1 * Rb2 - Rb1 - Rb2 + 1)/(2 * Tcol));
}

template <typename T, size_t Nz, size_t Nr, bool detector>
T funcToMinimizeMC(const T& a, const T& tau, const T& g, const Medium<T>& empty_tissue, const std::vector<Medium<T>>& slides, int Np, int threads,
                   T z, T r, const IntegratingSphere<T>& new_sphereR, const IntegratingSphere<T>& new_sphereT,
                   const DetectorDistances<T> new_dist, const std::vector<std::pair<T,T>>& rmeas, const std::vector<std::pair<T,T>>& tmeas) {
 //   auto tissue = Medium<T>::fromAlbedo(empty_tissue.n, a, tau, empty_tissue.D, g);
    std::vector<Medium<T>> layers;
    if (slides.empty())
        layers = {Medium<T>::fromAlbedo(empty_tissue.n, a, tau, empty_tissue.D, g)};
    else
        layers = {slides[0], Medium<T>::fromAlbedo(empty_tissue.n, a, tau, empty_tissue.D, g), slides[1]};
    Sample<T> sample(layers, 1.0, 1.0);

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(sample, Np, threads, z, r, myResultsMT, new_sphereR, new_sphereT, new_dist);

    auto rMC = myResultsMT.detectedR;
    auto tMC = myResultsMT.detectedT;
    T func2min = 0;

    constexpr auto eps = 1E-6;
    for (int i = 0; i < rMC.size(); i++) {
        func2min += fabs((rMC[i].second - rmeas[i].second)/(rmeas[i].second + eps)) + fabs((tMC[i].second - tmeas[i].second)/(tmeas[i].second + eps));
    }
    return func2min;
}

template <typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix>
class MinimizableMC {
public:
    virtual T funcToMinimize3argsMC(Matrix<T, 1, N> vec) const = 0;
};

template <typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix>
class funcMC: public MinimizableMC<T, Nz, Nr, detector, N, fix> {
public:
    funcMC(T fixed_param, const Medium<T>& new_empty_tissue, const std::vector<Medium<T>>& new_slides, int new_Np,
           int new_threads, T new_z, T new_r, const IntegratingSphere<T>& new_sphereR, const IntegratingSphere<T>& new_sphereT,
           const DetectorDistances<T> new_dist, const std::vector<std::pair<T,T>>& new_rmeas, const std::vector<std::pair<T,T>>& new_tmeas, const T& new_tcmeas) noexcept
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

    T funcToMinimize3argsMC(Matrix<T, 1, N> vec) const {
        if (N == 2) {
            if (fix)
                return funcToMinimizeMC<T, Nz, Nr, detector>(vec(0), this->tau, vec(1), this->empty_tissue, this->slides, this->Np, this->threads, this->z, this->r, this->SphereR, this->SphereT, this->dist, this->rmeas, this->tmeas);
            else
                return funcToMinimizeMC<T, Nz, Nr, detector>(vec(0), vec(1), this->g, this->empty_tissue, this->slides, this->Np, this->threads, this->z, this->r, this->SphereR, this->SphereT, this->dist, this->rmeas, this->tmeas);
        } else if (N == 3)
            return funcToMinimizeMC<T, Nz, Nr, detector>(vec(0), vec(1), vec(2), this->empty_tissue, this->slides, this->Np, this->threads, this->z, this->r, this->SphereR, this->SphereT, this->dist, this->rmeas, this->tmeas);
        else
            throw std::invalid_argument("N should be in range [2, 3]");
    }
    Medium<T> getEmptyTissue() const noexcept {return empty_tissue;}
    std::vector<Medium<T>> getSlides() const noexcept { return slides; }
    int getNp() const noexcept { return Np; }
    int getThreads() const noexcept { return threads; }
    T getZ() const noexcept { return z; }
    T getR() const noexcept { return r; }
    IntegratingSphere<T> getSphereR() const noexcept { return SphereR; }
    IntegratingSphere<T> getSphereT() const noexcept { return SphereT; }
    DetectorDistances<T> getDist() const noexcept { return dist; }
    std::vector<std::pair<T,T>> getRmeas() const noexcept { return rmeas; }
    std::vector<std::pair<T,T>> getTmeas() const noexcept { return tmeas; }
    T getTcmeas() const noexcept { return tcmeas; }
    T getTau() const noexcept { return tau; }
    T getG() const noexcept { return g; }

protected:
    T tau, g;

 //   Sample<T> sample;
    Medium<T> empty_tissue;
    std::vector<Medium<T>> slides;
    int Np, threads;
    T z, r, tcmeas;
    IntegratingSphere<T> SphereR, SphereT;
    DetectorDistances<T> dist;
    std::vector<std::pair<T,T>> rmeas, tmeas;

};

template <typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix>
T fixParam (T newG, Medium<T> empty_tissue, std::vector<Medium<T>> slides, T tcmeas) {
    T n_slab = empty_tissue.n;
    T n_slide_top, n_slide_bottom;
    if (slides.empty()) {
        n_slide_top = n_slab;
        n_slide_bottom = n_slab;
    } else {
        n_slide_top = slides[0].n;
        n_slide_bottom = slides[1].n;
    }
    if (fix) // fixed tau
        return tauCalc<T>(n_slab, n_slide_top, n_slide_bottom, tcmeas); // tau
    else // fixed g
        return newG; // g
}

template <typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix>
void NelderMeadMin(funcMC<T, Nz, Nr, detector, N, fix> f, int maxIter, T astart, T tstart, T gstart, Matrix<T, 1, N>& vecMin, T& fmin, int& iters) {
    Matrix<T, 1, N> vstart, vb, vg, vw, vmid, vr, ve, vc, vs, vprevious;
    T alpha = 1.0;
    T beta = 0.5;
    T gamma = 2.0;

    /// INITIALIZING STARTING SIMPLEX
    if (N == 3)
        vstart << astart, tstart, gstart;
    else if (N == 2)
        vstart << astart, gstart;
    std::array<Matrix<T, 1, N>, N> basis;
    std::array<Matrix<T, 1, N>, N+1> start;
    std::array<std::pair<Matrix<T, 1, N>, T>, N+1> simplex;
    for (size_t i = 0; i < N; i++) {
        basis[i] = Matrix<T, 1, N>::Zero();
        basis[i](i) = 1.0;
    }

    start[0] = vstart;
    simplex[0].first = start[0];
    for (size_t i = 1; i < N + 1; i++) {
        T h = 0;
        if (vstart(i-1) == 0)
            h = 0.0025;
        else
            h = 0.05;
        start[i] = start[i - 1] + h * basis[i - 1];
        simplex[i].first = start[i];
    }

    vprevious = simplex[0].first;

 /*   simplex[0].first << 0.866951, 0.903689;
    simplex[1].first << 0.849763, 0.906033;
    simplex[2].first << 0.857088, 0.902761;*/

    std::cout << simplex[0].first << " " << simplex[0].second << std::endl;
    std::cout << simplex[1].first << " " << simplex[1].second << std::endl;
    std::cout << simplex[2].first << " " << simplex[2].second << std::endl;

    for (int k = 0; k < maxIter; k++) {
        iters = k;
        T eps = 1e-4;
        std::cout << k << std::endl;

        /// FIND BEST, GOOD AND WORST VERTICES OF SIMPLEX
        for (size_t i = 0; i < N + 1; i++){
            simplex[i].second = f.funcToMinimize3argsMC(simplex[i].first);
            // std::cout << simplex[i].first << " " << simplex[i].second << std::endl;
        }
        std::sort(begin(simplex), end(simplex), sortSimplex<T, N>);
        vb = simplex[0].first;
        vg = simplex[1].first;
        vw = simplex[N].first;

        std::cout << simplex[0].first << " " << simplex[0].second << std::endl;
        std::cout << simplex[1].first << " " << simplex[1].second << std::endl;
        std::cout << simplex[2].first << " " << simplex[2].second << std::endl;

        /// FIND CENTER OF MASS OF EVERYTHING EXCEPT WORST VERTEX
        vmid = Matrix<T, 1, N>::Zero();
        for (size_t i = 0; i < N; i++){
            T sum = 0;
            for (size_t j = 0; j < N; j++)
                sum += simplex[j].first(i);
            sum /= N;
            vmid(i) = sum;
        }
        /// REFLECTION
        vr = (1 + alpha) * vmid - alpha*vw;
        /// IS VR A GOOD VERTEX?
        T fvr = f.funcToMinimize3argsMC(vr);
        if (fvr < f.funcToMinimize3argsMC(vb)){
            /// EXPANSION
            ve = (1 - gamma)*vmid + gamma*vr;
            if (f.funcToMinimize3argsMC(ve) < fvr) {
                simplex[N].first = ve;
                simplex[N].second = f.funcToMinimize3argsMC(ve);

                int checksum = checkConvergence<T,N>(simplex[N].first, vprevious, eps);
                if (checksum == N)
                    break;

                vprevious = simplex[N].first;
                std::cout << "v previous " << vprevious << std::endl;
                continue;
            } else {
                simplex[N].first = vr;
                simplex[N].second = fvr;

                int checksum = checkConvergence<T,N>(simplex[N].first, vprevious, eps);
                if (checksum == N)
                    break;

                vprevious = simplex[N].first;
                std::cout << "v previous " << vprevious << std::endl;
                continue;
            }
        } else if (f.funcToMinimize3argsMC(vb) < fvr && fvr < f.funcToMinimize3argsMC(vg)) {
            simplex[N].first = vr;
            simplex[N].second = f.funcToMinimize3argsMC(vr);

            int checksum = checkConvergence<T,N>(simplex[N].first, vprevious, eps);
            if (checksum == N)
                break;

            vprevious = simplex[N].first;
            std::cout << "v previous " << vprevious << std::endl;
            continue;
        } else if (f.funcToMinimize3argsMC(vg) < fvr && fvr < f.funcToMinimize3argsMC(vw)) {
            Matrix<T, 1, N> cache = vr;
            vr = vw;
            vw = cache;
        }
        /// SHRINK
        vs = beta*vw + (1 - beta)*vmid;
        if (f.funcToMinimize3argsMC(vs) < f.funcToMinimize3argsMC(vw)) {
            simplex[N].first = vs;
            simplex[N].second = f.funcToMinimize3argsMC(vs);

        int checksum = checkConvergence<T,N>(simplex[N].first, vprevious, eps);
            if (checksum == N)
                break;

            vprevious = simplex[N].first;
            std::cout << "v previous " << vprevious << std::endl;
            continue;
        } else {
            /// GLOBAL SHRINK;
            for (size_t i = 1; i <= N; i++) {
                simplex[i].first = simplex[0].first + (simplex[i].first - simplex[0].first)/2;
                simplex[i].second = f.funcToMinimize3argsMC(simplex[i].first);
            }
            vprevious = simplex[N].first;
            std::cout << "v previous " << vprevious << std::endl;

            int checksum = checkConvergence<T,N>(simplex[N].first, vprevious, eps);
            if (checksum == N)
                break;

            continue;
        }
    }

    std::sort(begin(simplex), end(simplex), sortSimplex<T, N>);
    // std::cout << "MINIMUM " << simplex[0].second << " AT POINT " << simplex[0].first << std::endl;
    vecMin = simplex[0].first;
    fmin = simplex[0].second;
}

template <typename T, size_t Nz, size_t Nr, bool detector, size_t N, bool fix>
void IMC(const std::vector<std::pair<T,T>>& rmeas, const std::vector<std::pair<T,T>>& tmeas, T tcmeas, const Medium<T>& empty_tissue, const std::vector<Medium<T>>& slides,
         int Np, int threads, T z, T r, const IntegratingSphere<T>& SphereR, const IntegratingSphere<T>& SphereT,
           const DetectorDistances<T>& dist, const T& aStart, const T& tStart, const T& gStart, T& aOut, T& tauOut, T& gOut) {
    T fixedParam = fixParam<T,Nz,Nr,detector,N,fix>(0.0, empty_tissue, slides, tcmeas);// fix == 1 => any arg, fix == 0 => value of g
    funcMC<T,Nz,Nr,detector,N,fix> toMinimize(fixedParam, empty_tissue, slides, Np, threads, z, r, SphereR, SphereT, dist, rmeas, tmeas, tcmeas);


    if (fix)
        std::cout << "Inverse Monte Carlo, fixed optical thickness = " << tStart << std::endl;
    else
        std::cout << "Inverse Monte Carlo, fixed anisotropy = " << gStart << std::endl;

    // std::cout << astart << " " << gstart << std::endl;

    int maxIter = 50;

    T fmin;
    Matrix<T, 1, N> vecMin;

    int itersMade;

    NelderMeadMin<T, Nz, Nr, detector, N, fix>(toMinimize, maxIter, aStart, tStart, gStart, vecMin, fmin, itersMade);

    std::cout << "Iterations made " << itersMade << std::endl;

 /*   if (itersMade == maxIter - 1) { //RESTART
        std::cout << "Restart" << std::endl;
        if (fix) {
            aStart = vecMin(0)+0.05;
            gStart = vecMin(1)-0.05;
        } else {
            aStart = vecMin(0)+0.05;
            tStart = vecMin(1)+1;
        }
        NelderMeadMin<T, Nz, Nr, detector, N, fix>(toMinimize, maxIter, aStart, tStart, gStart, vecMin, fmin, itersMade);
        std::cout << "Iterations made " << itersMade << std::endl;
    }*/

    if (fix){
        std::cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", g = " << vecMin(1) << ", tau = " << fixedParam << std::endl;
        aOut = vecMin(0);
        tauOut = fixedParam;
        gOut = vecMin(1);
    } else {
        std::cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << ", g = " << fixedParam <<std::endl;
        aOut = vecMin(0);
        tauOut = vecMin(1);
        gOut = fixedParam;
    }
}
