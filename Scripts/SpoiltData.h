#pragma once

#include "../Utils/ReadFiles.h"
#include "../Inverse/InverseProblem.h"
#include "../Inverse/StartingPoints.h"
#include "DirectMC.h"

#include <time.h>
#include <random>

template < typename T, size_t N, Inverse_NS::FixedParameter fix, size_t M, size_t Nz, size_t Nr, bool detector >
void spoiltData(T inA, T inT, T inG, T inN, T inD, T inNtop, T inDtop, T inNbottom, T inDbottom, bool moveable, int Nthreads, double err) {
    using namespace Inverse_NS;
    using namespace std;

    MCresults<T,Nz,Nr,detector> cleanRes = directMC<T,Nz,Nr,detector>(inA, inT, inG, inNtop, inDtop, inN, inD, inNbottom, inDbottom, moveable, Nthreads, 0);
    const vector<pair<T,T>> rsmeas = cleanRes.detectedR;
    const vector<pair<T,T>> tsmeas = cleanRes.detectedT;
    const vector<pair<T,T>> tcmeas = {make_pair(0.0, cleanRes.BugerTransmission)};
    vector<pair<T,T>> rSpoilt, tSpoilt, tcSpoilt;

    vector<Medium<T>> layers;
    auto emptyTissue = Medium<T>::fromAlbedo(inN, 0.0, 0.0, inD, 0.0);
    if (inDtop != 0 && inDbottom != 0) {
        auto glassTop = Medium<T>::fromCoeffs(inNtop, 0.0, 0.0, inDtop, 0.0);
        auto glassBottom = Medium<T>::fromCoeffs(inNbottom, 0.0, 0.0, inDbottom, 0.0);
        layers = {glassTop, emptyTissue, glassBottom};
    } else if (inDtop == 0 && inDbottom == 0)
        layers = {emptyTissue};
    else
        throw invalid_argument("It seems that you want to calculate 2 layers, you can only do 1 or 3");
    Sample<T> emptySample(layers);

    const IntegratingSphere<T> SphereR = cleanRes.mainSphereR;
    const IntegratingSphere<T> SphereT = cleanRes.mainSphereT;

    cerr << SphereR.getDPort1() << " " << SphereR.getDPort2() << " " << SphereR.getDSphere() << endl;
    cerr << SphereT.getDPort1() << " " << SphereT.getDPort2() << " " << SphereT.getDSphere() << endl;

    DetectorDistance<T> distances;
    distances.max  = T(rsmeas[rsmeas.size()-1].first);
    distances.min  = T(rsmeas[0].first);
    distances.step = T(rsmeas[rsmeas.size()-1].first - rsmeas[rsmeas.size() - 2].first); // please, enter correct step for your borders

    constexpr int Nphotons = 1E6;
    constexpr T selectedRadius = 10E-2;

    ofstream fout;
    if (moveable)
        fout.open("Output files/Error script/" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + "_" + to_string(err) + "_moveable.txt");
    else
        fout.open("Output files/Error script/" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + "_" + to_string(err) + "_fixed.txt");
    fout << "a, tau, g, time" << '\n';

    /// TODO: Utils\Random.h
    static default_random_engine generator(time(nullptr));
    normal_distribution<T> distribution(0, err);

    for (int i = 0; i < 20; i++) {
        clock_t begin = clock();
        T diff = 0;
        for (const auto& x: rsmeas) {
            T e1 = distribution(generator);
            rSpoilt.push_back({x.first, x.second + e1});
            diff += e1;
        }
        for (const auto& x: tsmeas) {
            T e2 = distribution(generator);
            tSpoilt.push_back({x.first, x.second + e2});
            diff += e2;
        }
        for (const auto& x: tcmeas) {
            T e3 = distribution(generator);
            tcSpoilt.push_back({x.first, x.second + e3});
            diff += e3;
        }
        cerr << "diff = " << diff << endl;

        for (const auto& x: rSpoilt)
            cerr << x.first << " " << x.second << endl;

        for (const auto& x: tSpoilt)
            cerr << x.first << " " << x.second << endl;

        inverseResults<T,Nz,Nr,detector> inverseRes = inverseMC<T,N,fix,M,Nz,Nr,detector>(rSpoilt, tSpoilt, tcSpoilt,
                                                                emptySample, SphereR, SphereT, distances,
                                                                Nphotons, selectedRadius, moveable, Nthreads, ModellingMethod::MC, 0);
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        fout << inverseRes.a << "\t" << inverseRes.tau << "\t" << inverseRes.g << "\t" << elapsed_secs << endl;

        rSpoilt.clear();
        tSpoilt.clear();
        tcSpoilt.clear();
    }
    fout.close();
}
