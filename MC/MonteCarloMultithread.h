#pragma once

#include "MonteCarlo.h"

#include "../Utils/Utils.h"

#include <thread>

/// TODO: why not return result?
template < typename T, size_t Nz, size_t Nr, bool detector >
void MCmultithread(const Sample<T>& sample,
                   int Np,
                   int threads,
                   T z,
                   T r,
                   MCresults<T,Nz,Nr,detector>& finalResults,
                   const IntegratingSphere<T>& sphereR,
                   const IntegratingSphere<T>& sphereT,
                   const DetectorDistance<T>& dist,
                   const LightSource<T>& source) {
    using namespace Physics_NS;
    using namespace Utils_NS;
    using namespace std;

    vector<MonteCarlo<T,Nz,Nr,detector>> mcDivided;
    vector<thread> mcThreads;
    vector<MCresults<T,Nz,Nr,detector>> mcResults;
    // MonteCarlo<T,Nz,Nr,detector> mc(sample, (Np / threads), z, r);
    for (int i = 0; i < threads; i++) {
        mcDivided.push_back(MonteCarlo<T,Nz,Nr,detector>(sample, (Np / threads), z, r, sphereR, sphereT, dist, source));
        mcResults.push_back(MCresults <T,Nz,Nr,detector>());
    }

    for (int i = 0; i < threads; i++)
        mcThreads.push_back(thread(&MonteCarlo<T,Nz,Nr,detector>::Calculate, ref(mcDivided[i]), ref(mcResults[i])));
    for (auto& thread: mcThreads)
        thread.join();

    for (const auto& result: mcResults) {
        finalResults.arrayR += result.arrayR;
        finalResults.arrayRspecular += result.arrayRspecular;
        finalResults.arrayT += result.arrayT;
        finalResults.matrixA += result.matrixA;
        finalResults.arrayAnglesR += result.arrayAnglesR;
        finalResults.arrayAnglesT += result.arrayAnglesT;
        finalResults.mainSphereR = result.mainSphereR;
        finalResults.mainSphereT = result.mainSphereT;
        finalResults.lightSource = result.lightSource;
        finalResults.sourceMatrix = result.sourceMatrix;

        if (detector == 1) {
            finalResults.detectedR.resize(result.detectedR.size());
            finalResults.detectedT.resize(result.detectedR.size());
            for (int i = 0; i < isize(finalResults.detectedR); i++) {
                finalResults.detectedR[i].second += result.detectedR[i].second;
                finalResults.detectedT[i].second += result.detectedT[i].second;
            }
        }
    }

    if (sample.getNlayers() == 1)
        finalResults.BugerTransmission = BugerLambert(sample.getMedium(0).getTau(), sample.getMedium(0).getN(), sample.getNvacLower(), sample.getNvacLower());
    else
        finalResults.BugerTransmission = BugerLambert(sample.getMedium(1).getTau(), sample.getMedium(1).getN(), sample.getMedium(0).getN(), sample.getMedium(2).getN());


    finalResults.diffuseReflection   = finalResults.arrayR.sum()         / Np;
    finalResults.specularReflection  = finalResults.arrayRspecular.sum() / Np;
    finalResults.diffuseTransmission = finalResults.arrayT.sum()         / Np;
    finalResults.absorbed            = finalResults.matrixA.sum()        / Np;
    finalResults.arrayAnglesT        = finalResults.arrayAnglesT         / Np;
    finalResults.arrayAnglesR        = finalResults.arrayAnglesR         / Np;

    for (int i = 0; i < isize(finalResults.detectedR); i++) {
        finalResults.detectedR[i].first = mcResults[0].detectedR[i].first;
        finalResults.detectedR[i].second /= threads;
        finalResults.detectedT[i].first = mcResults[0].detectedT[i].first;
        finalResults.detectedT[i].second /= threads;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector >
MCresults<T,Nz,Nr,detector> MCmultithread(const Sample<T>& sample, int Np, int threads, T z, T r,
                                          const IntegratingSphere<T>& sphereR, const IntegratingSphere<T>& sphereT,
                                          const DetectorDistance<T> dist, const LightSource<T> source) {
    MCresults<T,Nz,Nr,detector> finalResults;
    MCmultithread(sample, Np, threads, z, r, finalResults, sphereR, sphereT, dist, source);
    return finalResults;
}
