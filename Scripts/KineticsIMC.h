#pragma once

#include "../Inverse/InverseProblem.h"
#include "../Inverse/StartingPoints.h"
#include "../Utils/ReadFiles.h"
#include "InverseMC.h"

#include <iostream>
template < typename T>
struct inverseResultsCoeffs {
    T mua;
    T mus;
    T g;
};


template < typename T, size_t N, Inverse_NS::FixedParameter fix, size_t M, size_t Nz, size_t Nr, bool detector >
void kineticsIMC(const std::string& settingsFile, const int& Nthreads, Inverse_NS::ModellingMethod startMod, const bool& save) {
    using namespace Inverse_NS;
    using namespace Physics_NS;
    using namespace std;

    int Nphotons;
    constexpr T selectedRadius = 10E-2;
    Sample<T> emptySample;
    IntegratingSphere<T> SphereR, SphereT;
    bool moveable;
    vector<pair<T,T>> Rd, Td, Tc;
    vector<pair<T,inverseResultsCoeffs<T>>> resultTable;
    vector<T> kineticTimes;
    LightSource<T> source;
    readSettings<T, fix>(settingsFile, emptySample, SphereR, SphereT, Nphotons, Rd, Td, Tc, source, 1, kineticTimes);

    DetectorDistance<T> distances;
    distances.max  = Td[0].first;
    distances.min  = Td[0].first;
    distances.step = 0.0;

    T g_val = 0.0;

    ofstream resultFile;
    resultFile.open("Output files/kinetics" + to_string(Rd[0].second) + "_" + to_string(Td[0].second) + "_" + to_string(Tc[0].second) + ".txt");
    if (!resultFile.is_open())
        throw invalid_argument("Failed to open file results");

    if (fix == FixedParameter::G && N == 2) {
        cerr << "Enter g " << endl;
        cin >> g_val;
    }
    for (int i = 0; i < Rd.size(); i++) {
        vector<pair<T,T>> newRd, newTd, newTc;
        newRd.push_back(Rd[i]);
        newTd.push_back(Td[i]);
        newTc.push_back(Tc[i]);
        inverseResults<T,Nz,Nr,detector> inverseResultsFin = inverseMC<T,N,fix,M,Nz,Nr,detector>(newRd, newTd, newTc, emptySample, SphereR, SphereT, source,
                                                                                             distances, Nphotons, selectedRadius, Nthreads, g_val, startMod, 0);

        inverseResultsCoeffs<T> res;
        res.g = inverseResultsFin.g;
        res.mua = (1 - inverseResultsFin.a) * inverseResultsFin.tau / emptySample.getTurbidMedium().getD();
        res.mus = inverseResultsFin.a       * inverseResultsFin.tau / emptySample.getTurbidMedium().getD();
        resultTable.push_back({kineticTimes[i], res});
        resultFile << resultTable[i].first << "\t" << resultTable[i].second.mus << "\t" << resultTable[i].second.mua << "\t" << resultTable[i].second.g << endl;
    }
    for (int i = 0; i < resultTable.size(); i++)
        cerr << resultTable[i].first << " : mus = " << resultTable[i].second.mus << " , mua = " << resultTable[i].second.mua << " , g = " << resultTable[i].second.g << endl;

//    return inverseResultsFin;
}
