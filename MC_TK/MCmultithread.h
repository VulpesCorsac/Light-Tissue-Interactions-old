#pragma once

#include "MonteCarlo.h"

#include <thread>

template < typename T, size_t Nz, size_t Nr >

void MCmultithread(const Sample<T>& sample, const int& Np, const int& threads, const T& z, const T& r, MCresults<T,Nz,Nr>& finalResults) {
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
    /// TODO: range-based for
    for (int i = 0; i < threads; i++)
        mcThreads[i].join();

    /// TODO: range-based for
    for (int i = 0; i < threads; i++) {
        finalResults.arrayR += mcResults[i].arrayR;
        finalResults.arrayRspecular += mcResults[i].arrayRspecular;
        finalResults.arrayT += mcResults[i].arrayT;
        finalResults.matrixA += mcResults[i].matrixA;
    }

    finalResults.diffuseReflection = finalResults.arrayR.sum() / Np;
    finalResults.specularReflection = finalResults.arrayRspecular.sum() / Np;
    finalResults.diffuseTransmission = finalResults.arrayT.sum() / Np;
    finalResults.absorbed = finalResults.matrixA.sum() / Np;

    /*
    cout << "Diffuse reflection = " << finalResults.diffuseReflection << endl;
    cout << "Specular reflection = " << finalResults.specularReflection << endl;
    cout << "Diffuse transmission = " << finalResults.diffuseTransmission << endl;
    cout << "Absorbed fraction = " << finalResults.absorbed << endl;
    //*/
}
