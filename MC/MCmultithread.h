#pragma once

#include "MonteCarlo.h"

#include <thread>

/// TODO: why not return result?
template < typename T, size_t Nz, size_t Nr >
void MCmultithread(const Sample<T>& sample, int Np, int threads, T z, T r, MCresults<T,Nz,Nr>& finalResults) {
    using namespace std;

    vector<MonteCarlo<T, Nz, Nr>> mcDivided;
    vector<thread> mcThreads;
    vector<MCresults<T, Nz, Nr>> mcResults;
    // MonteCarlo<T, Nz, Nr> mc(sample, (Np / threads), z, r);
    for (int i = 0; i < threads; i++) {
        mcDivided.push_back(MonteCarlo<T, Nz, Nr>(sample, (Np / threads), z, r));
        mcResults.push_back(MCresults<T, Nz, Nr>());
    }

    for (int i = 0; i < threads; i++)
        mcThreads.push_back(thread(&MonteCarlo<T, Nz, Nr>::Calculate, ref(mcDivided[i]), ref(mcResults[i])));
    for (auto& thread: mcThreads)
        thread.join();

    for (const auto& result: mcResults) {
        finalResults.arrayR += result.arrayR;
        finalResults.arrayRspecular += result.arrayRspecular;
        finalResults.arrayT += result.arrayT;
        finalResults.matrixA += result.matrixA;
    }

    finalResults.diffuseReflection = finalResults.arrayR.sum() / Np;
    finalResults.specularReflection = finalResults.arrayRspecular.sum() / Np;
    finalResults.diffuseTransmission = finalResults.arrayT.sum() / Np;
    finalResults.absorbed = finalResults.matrixA.sum() / Np;
}

template < typename T, size_t Nz, size_t Nr >
MCresults<T,Nz,Nr> MCmultithread(const Sample<T>& sample, int Np, int threads, T z, T r) {
    MCresults<T,Nz,Nr> finalResults;
    MCmultithread(sample, Np, threads, z, r, finalResults);
    return finalResults;
}
