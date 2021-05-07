#include "MonteCarlo.h"

#include <thread>

template < typename T, size_t Nz, size_t Nr >

void MCmultithread (const Sample<T>& sample, const int& Np, const int& threads, const T& z, const T& r, MCresults<T,Nz,Nr>& finalResults) {
    std::vector<MonteCarlo<T, Nz, Nr>> mcDivided;
    std::vector<std::thread> mcThreads;
    std::vector<MCresults<T, Nz, Nr>> mcResults;
 //   MonteCarlo<T, Nz, Nr> mc(sample, (Np / threads), z, r);
    for (int i = 0; i < threads; i++){
        mcDivided.push_back(MonteCarlo<T, Nz, Nr>(sample, (Np / threads), z, r));
        mcResults.push_back(MCresults<T, Nz, Nr>());
    }
    for (int i = 0; i < threads; i++)
        mcThreads.push_back(std::thread(&MonteCarlo<T, Nz, Nr>::Calculate, std::ref(mcDivided[i]), std::ref(mcResults[i])));
    for (int i = 0; i < threads; i++)
        mcThreads[i].join();

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

/*    std::cout << "Diffuse reflection = " << finalResults.diffuseReflection << std::endl;
    std::cout << "Specular reflection = " << finalResults.specularReflection << std::endl;
    std::cout << "Diffuse transmission = " << finalResults.diffuseTransmission << std::endl;
    std::cout << "Absorbed fraction = " << finalResults.absorbed << std::endl;*/
}
