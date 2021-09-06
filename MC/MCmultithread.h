#pragma once

#include "MonteCarlo.h"

#include <thread>

/// TODO: why not return result?
template < typename T, size_t Nz, size_t Nr, bool detector >
void MCmultithread(const Sample<T>& sample, int Np, int threads, T z, T r, MCresults<T,Nz,Nr,detector>& finalResults, const IntegratingSphere<T>& new_sphereR, const IntegratingSphere<T>& new_sphereT, const DetectorDistances<T> new_dist) {
    using namespace std;

    vector<MonteCarlo<T,Nz,Nr,detector>> mcDivided;
    vector<thread> mcThreads;
    vector<MCresults<T,Nz,Nr,detector>> mcResults;
    // MonteCarlo<T,Nz,Nr,detector> mc(sample, (Np / threads), z, r);
    for (int i = 0; i < threads; i++) {
        mcDivided.push_back(MonteCarlo<T,Nz,Nr,detector>(sample, (Np / threads), z, r, new_sphereR, new_sphereT, new_dist));
        mcResults.push_back(MCresults<T,Nz,Nr,detector>());
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
        finalResults.BugerTransmission = BugerLambert(sample.getMedium(0).tau, sample.getMedium(0).n, sample.getNvacLower(), sample.getNvacLower());
    else
        finalResults.BugerTransmission = BugerLambert(sample.getMedium(1).tau, sample.getMedium(1).n, sample.getMedium(0).n, sample.getMedium(2).n);


    finalResults.diffuseReflection = finalResults.arrayR.sum() / Np;
    finalResults.specularReflection = finalResults.arrayRspecular.sum() / Np;
    finalResults.diffuseTransmission = finalResults.arrayT.sum() / Np;
    finalResults.absorbed = finalResults.matrixA.sum() / Np;

    for (int i = 0; i < isize(finalResults.detectedR); i++) {
        finalResults.detectedR[i].first = mcResults[0].detectedR[i].first;
        finalResults.detectedT[i].first = mcResults[0].detectedT[i].first;
        finalResults.detectedR[i].second /= threads;
        finalResults.detectedT[i].second /= threads;
    }
}

template < typename T, size_t Nz, size_t Nr, bool detector >
MCresults<T,Nz,Nr,detector> MCmultithread(const Sample<T>& sample, int Np, int threads, T z, T r, const IntegratingSphere<T>& new_sphereR, const IntegratingSphere<T>& new_sphereT, const DetectorDistances<T> new_dist) {
    MCresults<T,Nz,Nr,detector> finalResults;
    MCmultithread(sample, Np, threads, z, r, finalResults, new_sphereR, new_sphereT, new_dist);
    return finalResults;
}
