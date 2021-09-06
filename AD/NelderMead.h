#pragma once

#include "IAD.h"

#include "../eigen/Eigen/Dense"

#include <iostream>
#include <utility>

template < typename T, size_t N >
bool sortSimplex(const std::pair<Matrix<T,1,N>, T> &a, const std::pair<Matrix<T,1,N>, T> &b) {
    return a.second < b.second;
}

template < typename T, size_t N >
int checkConvergence(const Matrix<T,1,N>& currentVec, const Matrix<T,1,N>& prevVec, const T& eps) {
    int checksum = 0;
    if (currentVec == prevVec)
        return 0;
    for (size_t m = 0; m < N; m++){
        if (std::abs(currentVec(m) - prevVec(m)) < eps)
             checksum += 1;
    }
    return checksum;
}

template < typename T >
T a2aComp(T a) {
    return (2*a - 1)/(a * (1 - a));
}

template < typename T >
T tau2tauComp(T tau) {
    return std::log(tau);
}

template < typename T >
T g2gComp(T g) {
    return (g /(1 - std::abs(g)));
//    return (2*g - 1)/(g * (1 - g));
}

template < typename T >
T aComp2a(T aC) {
    return (std::sqrt(sqr(aC) + 4) + aC - 2) / (2 * aC);
}

template < typename T >
T tauComp2tau(T tauC) {
    return std::exp(tauC);
}

template < typename T >
T gComp2g(T gC) {
    return (gC/(1 + std::abs(gC)));
 //   return (std::sqrt(sqr(gC) + 4) + gC - 2) / (2 * gC);
}

template < typename T, size_t N, bool fix >
Matrix<T,1,N> v2vComp (Matrix<T,1,N> v) {
    Matrix<T,1,N> vComp;
    if (N == 3)
        vComp << a2aComp<T>(v(0)), tau2tauComp<T>(v(1)), g2gComp<T>(v(2));
    else if (N == 2) {
        if (fix)
            vComp << a2aComp<T>(v(0)), g2gComp<T>(v(1));
        else
            vComp << a2aComp<T>(v(0)), tau2tauComp<T>(v(1));
    }
    return vComp;
}

template < typename T, size_t N, bool fix >
Matrix<T,1,N> vComp2v (Matrix<T,1,N> vComp) {
    Matrix<T,1,N> v;
    if (N == 3)
        v << aComp2a<T>(vComp(0)), tauComp2tau<T>(vComp(1)), gComp2g<T>(vComp(2));
    else if (N == 2) {
        if (fix)
            v << aComp2a<T>(vComp(0)), gComp2g<T>(vComp(1));
        else
            v << aComp2a<T>(vComp(0)), tauComp2tau<T>(vComp(1));
    }
    return v;
}

template <typename T, size_t M, size_t N, bool fix>
void NelderMeadMin(const func<T, M, N, fix>& f, int maxIter, T astart, T tstart, T gstart, Matrix<T,1,N>& vecMin, T& fmin, int& iters) {
    Matrix<T,1,N> vstart, vb, vg, vw, vmid, vr, ve, vc, vs, vprevious;
    T alpha = 1.0;
    T beta = 0.5;
    T gamma = 2.0;

    /// INITIALIZING STARTING SIMPLEX

    if (N == 3)
        vstart << astart, tstart, gstart;
    else if (N == 2) {
        if (fix)
            vstart << astart, gstart;
        else
            vstart << astart, tstart;
    }
    std::array<Matrix<T,1,N>,N> basis;
    std::array<Matrix<T,1,N>,N+1> start;
    std::array<std::pair<Matrix<T,1,N>,T>,N+1> simplex;
    for (size_t i = 0; i < N; i++) {
        basis[i] = Matrix<T,1,N>::Zero();
        basis[i](i) = 1.0;
    }
    std::cout << "START " << astart << " " << tstart << " " << gstart << std::endl;

    start[0] = vstart;
    simplex[0].first = v2vComp<T,N,fix>(start[0]);
    for (size_t i = 1; i < N + 1; i++) {
        T h = 0;
        if (vstart(i-1) == 0)
            h = 0.0025;
        else if (N == 2 && fix && (vstart(0) >= 0.95 || vstart(1) >= 0.95))
            h = -0.05;
        else
            h = +0.05;
        start[i] = start[i - 1] - h * basis[i - 1];
        simplex[i].first = v2vComp<T,N,fix>(start[i]);
    }

    vprevious = vComp2v<T,N,fix>(simplex[0].first);

    for (int k = 0; k < maxIter; k++) {
        iters = k;
        T eps = 1E-5; // for checking convergence

        /// FIND BEST, GOOD AND WORST VERTICES OF SIMPLEX
        for (size_t i = 0; i < N + 1; i++) {
            simplex[i].second = f.funcToMinimize3args(vComp2v<T,N,fix>(simplex[i].first));
            // std::cout << simplex[i].first << " " << simplex[i].second << std::endl;
        }
        std::sort(begin(simplex), end(simplex), sortSimplex<T, N>);
        vb = simplex[0].first;
        vg = simplex[1].first;
        vw = simplex[N].first;

        /*
        std::cout << simplex[0].first << " " << vComp2v<T,N,fix>(simplex[0].first) << " " << simplex[0].second << std::endl;
        std::cout << simplex[1].first << " " << vComp2v<T,N,fix>(simplex[1].first) << " " << simplex[1].second << std::endl;
        std::cout << simplex[2].first << " " << vComp2v<T,N,fix>(simplex[2].first) << " " << simplex[2].second << std::endl;
        //*/

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
        T fvr = f.funcToMinimize3args(vComp2v<T,N,fix>(vr));

        /*
        std::cout << "Vmid " << vmid << " " << vComp2v<T,N,fix>(vmid) << std::endl;
        std::cout << "VR " << vr << " " << vComp2v<T,N,fix>(vr) << " " << fvr << std::endl;
        //*/

        if (fvr < f.funcToMinimize3args(vComp2v<T,N,fix>(vb))) {
            /// EXPANSION
            ve = (1 - gamma)*vmid + gamma*vr;
            if (f.funcToMinimize3args(vComp2v<T,N,fix>(ve)) < fvr) {
                simplex[N].first = ve;
                simplex[N].second = f.funcToMinimize3args(vComp2v<T,N,fix>(ve));

                int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                /*
                std::cout << "v previous " << vprevious << std::endl;
                std::cout << "expanded" << std::endl;
                //*/
                continue;
            } else {
                simplex[N].first = vr;
                simplex[N].second = fvr;

                int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                /*
                std::cout << "v previous " << vprevious << std::endl;
                std::cout << "1" << std::endl;
                //*/
                continue;
            }
        } else if (f.funcToMinimize3args(vComp2v<T,N,fix>(vb)) < fvr && fvr < f.funcToMinimize3args(vComp2v<T,N,fix>(vg))) {
            simplex[N].first = vr;
            simplex[N].second = f.funcToMinimize3args(vComp2v<T,N,fix>(vr));

            int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            /*
            std::cout << "v previous " << vprevious << std::endl;
            std::cout << "2" << std::endl;
            //*/
            continue;
        } else if (f.funcToMinimize3args(vComp2v<T,N,fix>(vg)) < fvr && fvr < f.funcToMinimize3args(vComp2v<T,N,fix>(vw))) {
            Matrix<T,1,N> cache = vr;
            vr = vw;
            vw = cache;
        }
        /// SHRINK
        vs = beta*vw + (1 - beta)*vmid;
        // std::cout << "VS " << vs << " " << vComp2v<T,N,fix>(vs) << " " << f.funcToMinimize3args(vComp2v<T,N,fix>(vs)) << std::endl;

        if (f.funcToMinimize3args(vComp2v<T,N,fix>(vs)) < f.funcToMinimize3args(vComp2v<T,N,fix>(vw))) {
            simplex[N].first = vs;
            simplex[N].second = f.funcToMinimize3args(vComp2v<T,N,fix>(vs));

            int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            /*
            std::cout << "v previous " << vprevious << std::endl;
            std::cout << "shrink" << std::endl;
            //*/
            continue;
        } else {
            /// GLOBAL SHRINK;
            for (size_t i = 1; i <= N; i++) {
                simplex[i].first = simplex[0].first + (simplex[i].first - simplex[0].first)/2;
                simplex[i].second = f.funcToMinimize3args(vComp2v<T,N,fix>(simplex[i].first));
            }

            int checksum = checkConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            /*
            std::cout << "v previous " << vprevious << std::endl;
            std::cout << "global shrink" << std::endl;
            //*/
            continue;
        }
    }

    std::sort(begin(simplex), end(simplex), sortSimplex<T, N>);
    // std::cout << "MINIMUM " << simplex[0].second << " AT POINT " << simplex[0].first << std::endl;
    vecMin = vComp2v<T,N,fix>(simplex[0].first);
    fmin = simplex[0].second;
}

template <typename T, size_t M, size_t N, bool fix>
void IAD(T rsmeas, T tsmeas, T tcmeas, T n_slab, T n_slide_top, T n_slide_bottom, T& aOut, T& tauOut, T& gOut) {
    T fixedParam = fixParam<T,M,N,fix>(0.0, n_slab, n_slide_top, n_slide_bottom, tcmeas);// fix == 1 => any arg, fix == 0 => value of g
    func<T, M, N, fix> toMinimize(fixedParam, n_slab, n_slide_top, n_slide_bottom, rsmeas, tsmeas, tcmeas);

    /// STARTING POINT
    T astart, tstart, gstart;
    startingPoints(toMinimize, astart, tstart, gstart);

    if (fix)
        std::cout << "Inverse Adding-Doubling, fixed optical thickness = " << tstart << std::endl;
    else
        std::cout << "Inverse Adding-Doubling, fixed anisotropy = " << gstart << std::endl;

    // std::cout << astart << " " << gstart << std::endl;

    int maxIter = 100;

    T fmin;
    Matrix<T,1,N> vecMin;

    int itersMade;

    NelderMeadMin<T,M,N,fix>(toMinimize, maxIter, astart, tstart, gstart, vecMin, fmin, itersMade);

    std::cout << "Iterations made " << itersMade << std::endl;

    /*
    if (itersMade == maxIter - 1) { //RESTART
        std::cout << "Restart" << std::endl;
        if (fix) {
            astart = vecMin(0)+0.05;
            gstart = vecMin(1)-0.05;
        } else {
            astart = vecMin(0)+0.05;
            tstart = vecMin(1)+1;
        }
        NelderMeadMin<T,M,N,fix>(toMinimize, maxIter, astart, tstart, gstart, vecMin, fmin, itersMade);
        std::cout << "Iterations made " << itersMade << std::endl;
    }
    //*/

    if (fix) {
        // std::cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", g = " << vecMin(1) << ", tau = " << fixedParam << std::endl;
        aOut = vecMin(0);

        // aOut = vecMin(0);
        tauOut = fixedParam;

        gOut = vecMin(1);
        // gOut = vecMin(1);
    } else {
        // std::cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << ", g = " << fixedParam <<std::endl;
        aOut = vecMin(0);

        // aOut = vecMin(0);
        tauOut = vecMin(1);
        gOut = fixedParam;
    }
}
