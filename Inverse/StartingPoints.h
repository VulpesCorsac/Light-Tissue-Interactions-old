#pragma once

#include "InverseProblem.h"

#include "../Physics/Reflectance.h"
#include "../Math/Basic.h"
#include "../eigen/Eigen/Dense"
#include "../AD/Eigen_NS.h"

template < typename T, size_t gSize>
void constructGrid(Matrix<T,1,gSize>& gridA, Matrix<T,1,gSize>& gridT, Matrix<T,1,gSize>& gridG) {
    using namespace Math_NS;
    using namespace std;

    T tMin = -3;
    T tMax = 3;

    for (size_t i = 0; i < gSize; i++) {
        const T j = i;
        const T x = j / (gSize - 1.0);
        gridA(i) = 0.9999 * (1 - x) + 0.00001;
      /*  if (x < 0.25)
            gridA(i) = 0.9999 * (1 - x) + 0.00001;
        else if (x > 0.75)
            gridA(i) = 0.9999 * (1 - x) + 0.00001;
        else
            gridA(i) = 0.9999 * sqr(1 - x) + 0.00001;*/
        gridT(i) = pow(2, tMin + (tMax - tMin) * x);
        /* gridG(i) = 0.9999 * (2.0 * i / (gSize - 1.0) - 1.0) + 0.00001;*/ /// including g < 0
        gridG(i) = 0.9999 * i / (gSize - 1.0) + 0.00001;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t M, size_t N, Inverse_NS::FixedParameter fix, size_t gSize >
Matrix<T,gSize,gSize> distances(const Func<T,Nz,Nr,detector,M,N,fix>& f,
                                const Matrix<T,1,gSize>& gridA, const Matrix<T,1,gSize>& gridT, const Matrix<T,1,gSize>& gridG,
                                const T& g, Inverse_NS::ModellingMethod mod) {
    using namespace Inverse_NS;
    using namespace AddingDoubling_NS;
    using namespace Physics_NS;
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(mod == ModellingMethod::AD || mod == ModellingMethod::MC);
    CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);
    CHECK_ARGUMENT_CONTRACT(f.getEmptySample().getNlayers() == 1 || f.getEmptySample().getNlayers() == 3);

    Matrix<T,gSize,gSize> distMatrix;

    Quadrature<T,M> quadStart(f.getEmptySample().getNslab());
    const auto vStart = quadStart.getV();
    const auto wStart = quadStart.getW();

    constexpr int Nphotons = 1000;
    constexpr int Nthreads = 1;
    DetectorDistance<T> dist;
    dist.max = f.getDistances().min;
    dist.min = f.getDistances().min;
    dist.step = 0.0;

    T tMeas = f.getTmeas()[0].second;
    T rMeas = f.getRmeas()[0].second;

    if (mod == ModellingMethod::AD) {
        rMeas += f.getRspec();
        if (f.getSphereT().getDPort2() != 0)
            tMeas += f.getTcmeas();
    }
    for (size_t i = 0; i < gSize; i++)
        cerr << "-";
    cerr << endl;
    constexpr T EPS = 1E-6;
    for (size_t i = 0; i < gSize; i++) {
        for (size_t j = 0; j < gSize; j++) {
            T tCalc = 0;
            T rCalc = 0;
            MCresults<T,Nz,Nr,detector> myResults;
            Medium<T> layer;
            if (N == 2) {
                if (fix == FixedParameter::Tau)
                    layer = Medium<T>::fromAlbedo(f.getEmptySample().getNslab(), gridA(i), f.getTau(), f.getEmptySample().getTurbidMedium().getD(), gridG(j));
                else if (fix == FixedParameter::G)
                    layer = Medium<T>::fromAlbedo(f.getEmptySample().getNslab(), gridA(i), gridT(j), f.getEmptySample().getTurbidMedium().getD(), f.getG());
            } else if (N == 3)
                layer = Medium<T>::fromAlbedo(f.getEmptySample().getNslab(), gridA(i), gridT(j), f.getEmptySample().getTurbidMedium().getD(), g);

            vector<Medium<T>> samples;
            if (f.getEmptySample().getNlayers() == 1)
                samples = {layer};
            else if (f.getEmptySample().getNlayers() == 3)
                samples = {f.getGlassTop(), layer, f.getGlassBottom()};
            else
                throw invalid_argument("Only one or three layers possible");

            if (mod == ModellingMethod::AD)
                RTs<T,M>(Sample<T>(samples), vStart, wStart, rCalc, tCalc);
            else if (mod == ModellingMethod::MC) {
                MCmultithread(Sample<T>(samples), Nphotons, Nthreads, f.getZ(), f.getR(),
                              myResults, f.getSphereR(), f.getSphereT(), dist, f.getLightSource());
                tCalc = myResults.detectedT[0].second;
                rCalc = myResults.detectedR[0].second;
            }
            distMatrix(i,j) = abs(rCalc - rMeas) / (rMeas + EPS) + abs(tCalc - tMeas) / (tMeas + EPS);
        //    cerr << gridA(i) << " " << gridT(j) << " " << distMatrix(i,j) << endl;
        }
        cerr << "#";
    }
    cerr << endl;
    return distMatrix;
}

template < typename T, size_t Nz, size_t Nr, bool detector, size_t M, size_t N, Inverse_NS::FixedParameter fix >
void startingPoints(const Func<T,Nz,Nr,detector,M,N,fix>& f, T& aStart, T& tStart, T& gStart, Inverse_NS::ModellingMethod mod) {
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);
    CHECK_ARGUMENT_CONTRACT(mod == ModellingMethod::AD || mod == ModellingMethod::MC);

    constexpr int gridSize = 30;
    Matrix<T, 1, gridSize> gridA, gridT, gridG;
    constructGrid<T,gridSize>(gridA, gridT, gridG);
    cerr << "Calculating starting grid..." << endl;
    if (N == 2) {
        const auto distancesMatrix = distances<T,Nz,Nr,detector,M,N,fix,gridSize>(f, gridA, gridT, gridG, 0.0, mod);
        int minRow, minCol;
        const T mins = distancesMatrix.minCoeff(&minRow, &minCol);
        ignore = mins;
        if (fix == FixedParameter::Tau) {
            aStart = gridA(minRow);
            gStart = gridG(minCol);
            tStart = f.getTau();
            cerr << f.getTau() << ": " << distancesMatrix.minCoeff() << " " << minRow << " " << minCol << endl;
            cerr << gridA(minRow) << " " << gridG(minCol) << endl;
        } else if (fix == FixedParameter::G) {
            aStart = gridA(minRow);
            tStart = gridT(minCol);
            gStart = f.getG();
            cerr << f.getG() << ": " << distancesMatrix.minCoeff() << " " << minRow << " " << minCol << endl;
        }
    } else if (N == 3) {
        vector<T> vectorMins;
        vector<pair<int,int>> vectorCoord;
        for (int i = 0; i < gridSize; i++) {
            const auto distancesMatrix = distances<T,Nz,Nr,detector,M,N,fix,gridSize>(f, gridA, gridT, gridG, gridG(i), mod);
            int minRow, minCol;
            const T mins = distancesMatrix.minCoeff(&minRow, &minCol);
            const T minHere = distancesMatrix.minCoeff();
            vectorMins.push_back(minHere);
            vectorCoord.push_back({minRow, minCol});
            ignore = mins;
            cerr << gridA(minRow) << " " << gridT(minCol) <<" " <<  gridG(i) << ": " << minHere << endl;
        }
        int minElementIndex = min_element(vectorMins.begin(),vectorMins.end()) - vectorMins.begin();
        aStart = gridA(vectorCoord[minElementIndex].first);
        tStart = gridT(vectorCoord[minElementIndex].second);
        gStart = gridG(minElementIndex);
    }
}



