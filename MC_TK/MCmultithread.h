#include "MonteCarlo.h"

#include <thread>

template < typename T, size_t Nz, size_t Nr >

void MCmultithread (const Sample<T>& sample, const int& Np, const int& threads, const T& z, const T& r) {
    std::vector<MonteCarlo<T, Nz, Nr>> mcDivided;
    std::vector<std::thread> mcThreads;
 //   MonteCarlo<T, Nz, Nr> mc(sample, (Np / threads), z, r);
    for (int i = 0; i < threads; i++) {
        std::cout << "Create threads" << std::endl;
        mcDivided.push_back(MonteCarlo<T, Nz, Nr>(sample, (Np / threads), z, r));
        mcThreads.push_back(std::thread(&MonteCarlo<T, Nz, Nr>::Calculate, std::ref(mcDivided[i])));
    }
    for (int i = 0; i < threads; i++) {
        std::cout << "Join threads" << std::endl;
        mcThreads[i].join();
    }

    MCresults<T,Nz,Nr> finalResults;

    for (int i = 0; i < threads; i++) {
        finalResults.arrayR += mcDivided[i].getArrayR();
        finalResults.arrayRspecular += mcDivided[i].getArrayRspec();
        finalResults.arrayT += mcDivided[i].getArrayT();
        finalResults.matrixA += mcDivided[i].getMatrixA();
    }
    finalResults.diffuseReflection = finalResults.arrayR.sum() / Np;
    finalResults.specularReflection = finalResults.arrayRspecular.sum() / Np;
    finalResults.diffuseTransmission = finalResults.arrayT.sum() / Np;
    finalResults.absorbed = finalResults.matrixA.sum() / Np;

    std::cout << "Diffuse reflection = " << finalResults.diffuseReflection << std::endl;
    std::cout << "Specular reflection = " << finalResults.specularReflection << std::endl;
    std::cout << "Diffuse transmission = " << finalResults.diffuseTransmission << std::endl;
    std::cout << "Absorbed fraction = " << finalResults.absorbed << std::endl;
}
