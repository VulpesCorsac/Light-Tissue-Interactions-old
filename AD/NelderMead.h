#pragma once

#include <Eigen/Dense>
#include <iostream>

#include "IAD.h"
#include <utility>


template <typename T, size_t N>
bool sortSimplex(const std::pair<Matrix<T, 1, N>, T> &a, const std::pair<Matrix<T, 1, N>, T> &b) {
    return (a.second < b.second);
}

template <typename T, size_t M, size_t N, bool fix>
void NelderMeadMin(func<T, M, N, fix> f, int maxIter, T astart, T tstart, T gstart, Matrix<T, 1, N>& vecMin, T& fmin) {

    Matrix<T, 1, N> vstart, vb, vg, vw, vmid, vr, ve, vc, vs;
    T alpha = 1.0;
    T beta = 0.5;
    T gamma = 2.0;

/*INITIALIZING STARTING SIMPLEX*/
    if (N == 3)
        vstart << astart, tstart, gstart;
    else if (N == 2)
        vstart << astart, gstart;
    std::array <Matrix<T, 1, N>, N> basis;
    std::array <Matrix<T, 1, N>, N+1> start;
    std::array <std::pair<Matrix<T, 1, N>, T>, N+1> simplex;
    for (int i = 0; i < N; i++) {
        basis[i] = Matrix<T, 1, N>::Zero();
        basis[i](i) = 1.0;
    }

    start[0] = vstart;
    simplex[0].first = start[0];
    for (int i = 1; i < N + 1; i++) {
        T h = 0;
        if (vstart(i-1) == 0)
            h = 0.0025;
        else
            h = 0.05;
        start[i] = start[i - 1] - h * basis[i - 1];
        simplex[i].first = start[i];
    }


    for (int k = 0; k < maxIter; k++) {
        // FIND BEST, GOOD AND WORST VERTICES OF SIMPLEX
        for (int i = 0; i < N + 1; i++){
            simplex[i].second = f.funcToMinimize3args(simplex[i].first);
//            std::cout << simplex[i].first << " " << simplex[i].second << std::endl;
        }
        std::sort(begin(simplex), end(simplex), sortSimplex<T, N>);
        vb = simplex[0].first;
        vg = simplex[1].first;
        vw = simplex[N].first;
        // FIND CENTER OF MASS OF EVERYTHING EXCEPT WORST VERTEX
        vmid = Matrix<T, 1, N>::Zero();
        for (int i = 0; i < N; i++){
            T sum = 0;
            for (int j = 0; j < N; j++) {
                sum += simplex[j].first(i);
            }
            sum /= N;
            vmid(i) = sum;
        }
        // REFLECTION
        vr = (1 + alpha) * vmid - alpha*vw;
        // IS VR A GOOD VERTEX?
        T fvr = f.funcToMinimize3args(vr);
        if (fvr < f.funcToMinimize3args(vb)){
            // EXPANSION
            ve = (1 - gamma)*vmid + gamma*vr;
            if (f.funcToMinimize3args(ve) < fvr) {
                simplex[N].first = ve;
                simplex[N].second = f.funcToMinimize3args(ve);
                continue;
            } else {
                simplex[N].first = vr;
                simplex[N].second = fvr;
                continue;
            }
        } else if (f.funcToMinimize3args(vb) < fvr && fvr < f.funcToMinimize3args(vg)) {
            simplex[N].first = vr;
            simplex[N].second = f.funcToMinimize3args(vr);
            continue;
        } else if (f.funcToMinimize3args(vg) < fvr && fvr < f.funcToMinimize3args(vw)) {
            Matrix<T, 1, N> cache = vr;
            vr = vw;
            vw = cache;
        }
        // SHRINK
        vs = beta*vw + (1 - beta)*vmid;
        if (f.funcToMinimize3args(vs) < f.funcToMinimize3args(vw)) {
            simplex[N].first = vs;
            simplex[N].second = f.funcToMinimize3args(vs);
            continue;
        } else {
          //  GLOBAL SHRINK;
            for (int i = 1; i < N; i++) {
                simplex[i].first = simplex[0].first + (simplex[i].first + simplex[0].first)/2;
                simplex[i].second = f.funcToMinimize3args(simplex[i].first);
            }
        }
    }
    std::sort(begin(simplex), end(simplex), sortSimplex<T, N>);
//    std::cout << "MINIMUM " << simplex[0].second << " AT POINT " << simplex[0].first << std::endl;
    vecMin = simplex[0].first;
    fmin = simplex[0].second;


}
