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

namespace AddingDoubling_NS {

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
    cerr << "START " << astart << " " << tstart << " " << gstart << endl;

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
        constexpr T EPS = 1E-5; /// for checking convergence

        /// FIND BEST, GOOD AND WORST VERTICES OF SIMPLEX
        for (size_t i = 0; i < N + 1; i++) {
            simplex[i].second = f.funcToMinimize3args(vComp2v<T,N,fix>(simplex[i].first));
            // cerr << simplex[i].first << " " << simplex[i].second << endl;
        }
        sort(ALL_CONTAINER(simplex), SortSimplex<T,N>);
        vb = simplex[0].first;
        vg = simplex[1].first;
        vw = simplex[N].first;

        /*
        cerr << simplex[0].first << " " << vComp2v<T,N,fix>(simplex[0].first) << " " << simplex[0].second << endl;
        cerr << simplex[1].first << " " << vComp2v<T,N,fix>(simplex[1].first) << " " << simplex[1].second << endl;
        cerr << simplex[2].first << " " << vComp2v<T,N,fix>(simplex[2].first) << " " << simplex[2].second << endl;
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
        cerr << "Vmid " << vmid << " " << vComp2v<T,N,fix>(vmid) << endl;
        cerr << "VR " << vr << " " << vComp2v<T,N,fix>(vr) << " " << fvr << endl;
        //*/

        if (fvr < f.funcToMinimize3args(vComp2v<T,N,fix>(vb))) {
            /// EXPANSION
            ve = (1 - gamma)*vmid + gamma*vr;
            if (f.funcToMinimize3args(vComp2v<T,N,fix>(ve)) < fvr) {
                simplex[N].first = ve;
                simplex[N].second = f.funcToMinimize3args(vComp2v<T,N,fix>(ve));

                const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                /*
                cerr << "v previous " << vprevious << endl;
                cerr << "expanded" << endl;
                //*/
                continue;
            } else {
                simplex[N].first = vr;
                simplex[N].second = fvr;

                const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
                if (checksum == N)
                    break;

                vprevious = vComp2v<T,N,fix>(simplex[N].first);
                /*
                cerr << "v previous " << vprevious << endl;
                cerr << "1" << endl;
                //*/
                continue;
            }
        } else if (f.funcToMinimize3args(vComp2v<T,N,fix>(vb)) < fvr && fvr < f.funcToMinimize3args(vComp2v<T,N,fix>(vg))) {
            simplex[N].first = vr;
            simplex[N].second = f.funcToMinimize3args(vComp2v<T,N,fix>(vr));

            const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            /*
            cerr << "v previous " << vprevious << endl;
            cerr << "2" << endl;
            //*/
            continue;
        } else if (f.funcToMinimize3args(vComp2v<T,N,fix>(vg)) < fvr && fvr < f.funcToMinimize3args(vComp2v<T,N,fix>(vw))) {
            Matrix<T,1,N> cache = vr;
            vr = vw;
            vw = cache;
        }
        /// SHRINK
        vs = beta*vw + (1 - beta)*vmid;
        // cerr << "VS " << vs << " " << vComp2v<T,N,fix>(vs) << " " << f.funcToMinimize3args(vComp2v<T,N,fix>(vs)) << endl;

        if (f.funcToMinimize3args(vComp2v<T,N,fix>(vs)) < f.funcToMinimize3args(vComp2v<T,N,fix>(vw))) {
            simplex[N].first = vs;
            simplex[N].second = f.funcToMinimize3args(vComp2v<T,N,fix>(vs));

            const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            /*
            cerr << "v previous " << vprevious << endl;
            cerr << "shrink" << endl;
            //*/
            continue;
        } else {
            /// GLOBAL SHRINK;
            for (size_t i = 1; i <= N; i++) {
                simplex[i].first = simplex[0].first + (simplex[i].first - simplex[0].first)/2;
                simplex[i].second = f.funcToMinimize3args(vComp2v<T,N,fix>(simplex[i].first));
            }

            const auto checksum = CheckConvergence<T,N>(vComp2v<T,N,fix>(simplex[N].first), vprevious, EPS);
            if (checksum == N)
                break;

            vprevious = vComp2v<T,N,fix>(simplex[N].first);
            /*
            cerr << "v previous " << vprevious << endl;
            cerr << "global shrink" << endl;
            //*/
            continue;
        }
    }

    sort(ALL_CONTAINER(simplex), SortSimplex<T,N>);
    // cerr << "MINIMUM " << simplex[0].second << " AT POINT " << simplex[0].first << endl;
    vecMin = vComp2v<T,N,fix>(simplex[0].first);
    fmin = simplex[0].second;
}

template < typename T, size_t M, size_t N, Inverse_NS::FixedParameter fix >
void IAD(Func<T,M,N,fix> toMinimize, T rsmeas, T tsmeas, T tcmeas, T nSlab, T nSlideTop, T nSlideBottom, T fixedParam, T astart, T tstart, T gstart, T& aOut, T& tauOut, T& gOut) {
    using namespace Inverse_NS;
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(2 <= N && N <= 3);
    CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

    if (fix == FixedParameter::Tau && N == 2)
        cerr << "Inverse Adding-Doubling, fixed optical thickness = " << tstart << endl;
    else if (fix == FixedParameter::G && N == 2)
        cerr << "Inverse Adding-Doubling, fixed anisotropy = " << gstart << endl;
	else if (N == 3)
		cerr << "Inverse Adding-Doubling N = 3" << endl;
    // cerr << astart << " " << gstart << endl;
    //


    int maxIter = 100;

    T fmin;
    Matrix<T,1,N> vecMin;

    int itersMade;
    NelderMeadMin<T,M,N,fix>(toMinimize, maxIter, astart, tstart, gstart, vecMin, fmin, itersMade);
    cerr << "Iterations made " << itersMade << endl;

/*
    if (itersMade == maxIter - 1) { //RESTART
        cerr << "Restart" << endl;
        if (fix == FixedParameter::Tau) {
            astart = vecMin(0)+0.05;
            gstart = vecMin(1)-0.05;
        } else {
            astart = vecMin(0)+0.05;
            tstart = vecMin(1)+1;
        }
        NelderMeadMin<T,M,N,fix>(toMinimize, maxIter, astart, tstart, gstart, vecMin, fmin, itersMade);
        cerr << "Iterations made " << itersMade << endl;
    }
    //*/


    if (N == 2) {
        if (fix == FixedParameter::Tau) {
            aOut = vecMin(0);
            tauOut = fixedParam;
            gOut = vecMin(1);
        } else if (fix == FixedParameter::G) {
            // cerr << "Minimum " << fmin << " at point a = " << vecMin(0) << ", tau = " << vecMin(1) << ", g = " << fixedParam <<endl;
            aOut = vecMin(0);
            tauOut = vecMin(1);
            gOut = fixedParam;
        }
    } else if (N == 3) {
        aOut = vecMin(0);
        tauOut = vecMin(1);
        gOut = vecMin(2);
    }
    //*/

/* else {
    //
        aOut = astart;
        gOut = gstart;
        tauOut = tstart;*/
    // }
}

}
