#pragma once

#include "../Inverse/InverseProblem.h"
#include "SaveResults.h"

template < typename T, size_t Nz, size_t Nr, bool detector>
MCresults<T,Nz,Nr,detector> directMC(T inA, T inT, T inG, T inNtop, T inDtop, T inN, T inD, T inNbottom, T inDbottom, bool moveable, int Nthreads, bool save) {
    using namespace Inverse_NS;
    using namespace std;

    vector<Medium<T>> layers;
    auto tissue = Medium<T>::fromAlbedo(inN, inA, inT, inD, inG);
    if (inDtop != 0 && inDbottom != 0) {
        auto glassTop = Medium<T>::fromCoeffs(inNtop, 0.0, 0.0, inDtop, 0.0);
        auto glassBottom = Medium<T>::fromCoeffs(inNbottom, 0.0, 0.0, inDbottom, 0.0);
        layers = {glassTop, tissue, glassBottom};
    } else if (inDtop == 0 && inDbottom == 0)
        layers = {tissue};
    else
        throw invalid_argument("It seems that you want to calculate 2 layers, you can only do 1 or 3");
    Sample<T> mySample(layers);
    LightSource<T> source(0.0, SourceType::Point);
    IntegratingSphere<T> SphereT(0.0508, 0.0125, 0.0); // dPort2 = zero if the sphere has one port
    IntegratingSphere<T> SphereR(0.0508, 0.0125, 0.0125);
    DetectorDistance<T> distances;
    distances.max  = moveable ? 0.1 : 0.0;
    distances.min  = 0;
    distances.step = 0.005; // please, enter correct step for your borders

    constexpr int Nphotons = 1E6;
    constexpr T selectedRadius = 10E-2;

    MCresults<T,Nz,Nr,detector> myResults;

    MCmultithread(mySample, Nphotons, Nthreads, mySample.getTotalThickness(), selectedRadius, myResults, SphereR, SphereT, distances, source);
    cout << myResults << endl;
    if (save)
        saveResults<T,Nz,Nr,detector>(myResults, inA, inT, inG, 0);

    return myResults;
}

