#pragma once

#include "InverseAddingDoubling.h"

#include "../Inverse/FixedParam.h"
#include "../Inverse/NelderMead.h"
#include "../Math/Basic.h"
#include "../Utils/Contracts.h"

#include "../eigen/Eigen/Dense"

#include <array>
#include <iostream>
#include <utility>

using namespace AddingDoubling_NS;
using namespace Inverse_NS;

template < typename T, size_t M, size_t N, Inverse_NS::FixedParameter fix >
void NelderMeadMin(const Func<T, M, N, fix>& f, int maxIter, T astart, T tstart, T gstart, Matrix<T,1,N>& vecMin, T& fmin, int& iters) {
    using namespace Inverse_NS;
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(2 <= N && N <= 3);

    Matrix<T,1,N> vstart, vb, vg, vw, vmid, vr, ve, vc, vs, vprevious;
    const T alpha = 1.0;
    const T beta  = 0.5;
    const T gamma = 2.0;

    /// INITIALIZING STARTING SIMPLEX

    if (N == 3)
        vstart << astart, tstart, gstart;
    else if (N == 2) {
        CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

        if (fix == FixedParameter::Tau)
            vstart << astart, gstart;
        else if (fix == FixedParameter::G)
            vstart << astart, tstart;
    }

    array<Matrix<T,1,N>,N> basis;
    array<Matrix<T,1,N>,N+1> start;
    array<pair<Matrix<T,1,N>,T>,N+1> simplex;
    for (size_t i = 0; i < N; i++) {
        basis[i] = Matrix<T,1,N>::Zero();
        basis[i](i) = 1.0;
    }
    cout << "START " << astart << " " << tstart << " " << gstart << endl;

    start[0] = vstart;
    simplex[0].first = v2vComp<T,N,fix>(start[0]);
    for (size_t i = 1; i < N + 1; i++) {
        T h = 0;
        if (vstart(i-1) == 0)
            h = 0.0025;
        else if (N == 2 && fix == FixedParameter::Tau && (vstart(0) >= 0.95 || vstart(1) >= 0.95))
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
            // cout << simplex[i].first << " " << simplex[i].second << endl;
        }
        sort(ALL_CONTAINER(simplex), SortSimplex<T, N>);
        vb = simplex[0].first;
        vg = simplex[1].first;
        vw = simplex[N].first;

        /*
        cout << simplex[0].first << " " << vComp2v<T,N,fix>(simplex[0].first) << " " << simplex[0].second << endl;
        cout << simplex[1].first << " " << vComp2v<T,N,fix>(simplex[1].first) << " " << simplex[1].second << endl;
        cout << simplex[2].first << " " << vComp2v<T,N,fix>(simplex[2].first) << " " << simplex[2].second << endl;
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
        const T fvr = f.funcToMinimize3args(vComp2v<T,N,fix>(vr));

        /*
        cout << "Vmid " << vmid << " " << vComp2v<T,N,fix>(vmid) << endl;
        cout << "VR " << vr << " " << vComp2v<T,N,fix>(vr) << " " << fvr << endl;
        //*/

        if (fvr < f.funcToMinimize3args(vComp2v<T,N,fix>(vb))) {
            /// EXPANSION
            ve = (1 - gamma)*vmid + gamma*vr;
            if (f.funcToMinimize3args(vComp2v<T,N,fix>(ve)) < fvr) {
                simplex[N].first = ve;
                simplex[N].second = f.funcToMinimize3args(vComp2v<T,N,fix>(ve));

                const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                /*
                cout << "v previous " << vprevious << endl;
                cout << "expanded" << endl;
                //*/
                continue;
            } else {
                simplex[N].first = vr;
                simplex[N].second = fvr;

                const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                /*
                cout << "v previous " << vprevious << endl;
                cout << "1" << endl;
                //*/
                continue;
            }
        } else if (f.funcToMinimize3args(vComp2v<T,N,fix>(vb)) < fvr && fvr < f.funcToMinimize3args(vComp2v<T,N,fix>(vg))) {
            simplex[N].first = vr;
            simplex[N].second = f.funcToMinimize3args(vComp2v<T,N,fix>(vr));

            const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            /*
            cout << "v previous " << vprevious << endl;
            cout << "2" << endl;
            //*/
            continue;
        } else if (f.funcToMinimize3args(vComp2v<T,N,fix>(vg)) < fvr && fvr < f.funcToMinimize3args(vComp2v<T,N,fix>(vw))) {
            Matrix<T,1,N> cache = vr;
            vr = vw;
            vw = cache;
        }
        /// SHRINK
        vs = beta*vw + (1 - beta)*vmid;
        // cout << "VS " << vs << " " << vComp2v<T,N,fix>(vs) << " " << f.funcToMinimize3args(vComp2v<T,N,fix>(vs)) << endl;

        if (f.funcToMinimize3args(vComp2v<T,N,fix>(vs)) < f.funcToMinimize3args(vComp2v<T,N,fix>(vw))) {
            simplex[N].first = vs;
            simplex[N].second = f.funcToMinimize3args(vComp2v<T,N,fix>(vs));

            const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            /*
            cout << "v previous " << vprevious << endl;
            cout << "shrink" << endl;
            //*/
            continue;
        } else {
            /// GLOBAL SHRINK;
            for (size_t i = 1; i <= N; i++) {
                simplex[i].first = simplex[0].first + (simplex[i].first - simplex[0].first)/2;
                simplex[i].second = f.funcToMinimize3args(vComp2v<T,N,fix>(simplex[i].first));
            }

            const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, eps);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            /*
            cout << "v previous " << vprevious << endl;
            cout << "global shrink" << endl;
            //*/
            continue;
        }
    }

    sort(ALL_CONTAINER(simplex), SortSimplex<T, N>);
    // cout << "MINIMUM " << simplex[0].second << " AT POINT " << simplex[0].first << endl;
    vecMin = vComp2v<T,N,fix>(simplex[0].first);
    fmin = simplex[0].second;
}

template < typename T, size_t M, size_t N, Inverse_NS::FixedParameter fix >
void IAD(T rsmeas, T tsmeas, T tcmeas, T nSlab, T n_slide_top, T n_slide_bottom, T& aOut, T& tauOut, T& gOut) {
    using namespace Inverse_NS;
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(2 <= N && N <= 3);
    CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

    T g_val = 0.0;
	if (fix == FixedParameter::G && N == 2){
        cout << "Enter g " << endl;
        cin >> g_val;
    }
    T fixedParam = fixParam<T,M,N,fix>(g_val, nSlab, n_slide_top, n_slide_bottom, tcmeas);// fix == 1 => any arg, fix == 0 => value of g
    Func<T,M,N,fix> toMinimize(fixedParam, nSlab, n_slide_top, n_slide_bottom, rsmeas, tsmeas, tcmeas);

    /// STARTING POINT
    T astart, tstart, gstart;
    startingPoints(toMinimize, astart, tstart, gstart);
    /*
    if (fix == FixedParameter::Tau && N == 2)
        cout << "Inverse Adding-Doubling, fixed optical thickness = " << tstart << endl;
    else if (fix == FixedParameter::G && N == 3)
        cout << "Inverse Adding-Doubling, fixed anisotropy = " << gstart << endl;
	else if (N == 3)
		cout << "Inverse Adding-Doubling N = 3" << endl;
    // cout << astart << " " << gstart << endl;
    //*/

    /*
    int maxIter = 100;

    T fmin;
    Matrix<T,1,N> vecMin;

    int itersMade;
    if (N == 3){
        NelderMeadMin<T,M,N,fix>(toMinimize, maxIter, astart, tstart, gstart, vecMin, fmin, itersMade);
    }
    cout << "Iterations made " << itersMade << endl;


    if (itersMade == maxIter - 1) { //RESTART
        cout << "Restart" << endl;
        if (fix) {
            astart = vecMin(0)+0.05;
            gstart = vecMin(1)-0.05;
        } else {
            astart = vecMin(0)+0.05;
            tstart = vecMin(1)+1;
        }
        NelderMeadMin<T,M,N,fix>(toMinimize, maxIter, astart, tstart, gstart, vecMin, fmin, itersMade);
        cout << "Iterations made " << itersMade << endl;
    }
    //*/

    /*
    if (N == 2) {
        if (fix == FixedParameter::Tau) {
            aOut = vecMin(0);
            tauOut = fixedParam;
            gOut = vecMin(1);
            // gOut = vecMin(1);
        } else if (fix == FixedParameter::G) {
            // cout << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << ", g = " << fixedParam <<endl;
            aOut = vecMin(0);

            // aOut = vecMin(0);
            tauOut = vecMin(1);
            gOut = fixedParam;
        }
    } else if (N == 3) {
        aOut = vecMin(0);
        tauOut = vecMin(1);
        gOut = vecMin(2);
    }
    //*/

    /*
    if (N == 3) {
        aOut = vecMin(0);
        tauOut = vecMin(1);
        gOut = vecMin(2);
    } else {
    //*/
        aOut = astart;
        gOut = gstart;
        tauOut = tstart;
    // }
}