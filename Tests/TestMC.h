#pragma once

#include "../MC_TK/MonteCarlo.h"
#include "../MC_TK/BugerLambert.h"

class TestsMC
{
    public:
        Tests() {}
        void SingleLayerAbsorptionOnly ();
        void MultiLayerAbsorptionOnly ();
        void SingleLayerAbsorptionScattering ();
        void MultiLayerAbsorptionScattering ();
};

/*
Я бы советовал сравнить следующие ситуации:
1) просто однослойный бугер
2) многослойное но только поглощение с AD
3) однослойное, но и рассеяние с AD
4) многослойное, но и рассеяние с AD
при этом на каждую позицию следующие вариации:
- 1 поток \ 4 потока
- коэффициент взаимодействия {много больше \ сопоставим \ много меньше} толщины ткани
*/

void TestsMC::SingleLayerAbsorptionScattering() {

    std::cout << "Single layer, absorption & scattering" << std::endl;

    using T = double;
    const int Nz = 1000;
    const int Nr = 10000;
    T selectedRadius = 10e-2;

    Medium<T> tissue1(1.5, 100, 900, 1e-3, 0.9); //MFP == D
    Medium<T> tissue2(1.3, 20, 80, 1e-3, 0.5); //MFP = 10 D
    Medium<T> tissue3(1.6, 700, 300, 10e-3, 0.0); //MFP == 0.1D, it will need more photons

    std::vector<Medium<T>> layer1 = {tissue1};
    std::vector<Medium<T>> layer2 = {tissue2};
    std::vector<Medium<T>> layer3 = {tissue3};

    Sample<T> sample1(layer1, 1.0, 1.0);
    Sample<T> sample2(layer2, 1.0, 1.0);
    Sample<T> sample3(layer3, 1.0, 1.0);

    MonteCarlo<T, Nz, Nr> mc1(sample1, 1e5, sample1.getTotalThickness(), selectedRadius);
    MonteCarlo<T, Nz, Nr> mc2(sample2, 1e5, sample2.getTotalThickness(), selectedRadius);
    MonteCarlo<T, Nz, Nr> mc3(sample3, 1e6, sample3.getTotalThickness(), selectedRadius);

    MCresults<T, Nz, Nr> res1, res2, res3;
    MCresults<T, Nz, Nr> MTres1, MTres2, MTres3;

    mc1.Calculate(res1);

    assert(std::abs(res1.specularReflection - 0.04) < 1e-4);
    assert(std::abs(res1.diffuseReflection - 0.0435) < 1e-3);
    assert(std::abs(res1.diffuseTransmission - 0.767) < 1e-3);
    assert(std::abs(res1.diffuseTransmission + res1.diffuseReflection +
                    res1.specularReflection + res1.absorbed - 1.0) < 1e-4);

    std::cout << "Test 1 OK: single thread" << std::endl;

    MCmultithread<T, Nz, Nr>(sample1, 1e5, 4, sample1.getTotalThickness(), selectedRadius, MTres1);

    assert(std::abs(MTres1.specularReflection - 0.04) < 1e-4);
    assert(std::abs(MTres1.diffuseReflection - 0.0435) < 1e-3);
    assert(std::abs(MTres1.diffuseTransmission - 0.767) < 1e-3);
    assert(std::abs(MTres1.diffuseTransmission + MTres1.diffuseReflection +
                    MTres1.specularReflection + MTres1.absorbed - 1.0) < 1e-4);

    std::cout << "Test 1 OK: 4 threads" << std::endl;

    mc2.Calculate(res2);

    assert(std::abs(res2.specularReflection - 0.01701) < 1e-4);
    assert(std::abs(res2.diffuseReflection - 0.0272) < 1e-3);
    assert(std::abs(res2.diffuseTransmission - 0.9206) < 1e-3);
    assert(std::abs(res2.diffuseTransmission + res2.diffuseReflection +
                    res2.specularReflection + res2.absorbed - 1.0) < 1e-4);

    std::cout << "Test 2 OK: single thread" << std::endl;

    MCmultithread<T, Nz, Nr>(sample2, 1e5, 4, sample2.getTotalThickness(), selectedRadius, MTres2);

    assert(std::abs(MTres2.specularReflection - 0.01701) < 1e-4);
    assert(std::abs(MTres2.diffuseReflection - 0.0272) < 1e-3);
    assert(std::abs(MTres2.diffuseTransmission - 0.9206) < 1e-3);
    assert(std::abs(MTres2.diffuseTransmission + MTres2.diffuseReflection +
                    MTres2.specularReflection + MTres2.absorbed - 1.0) < 1e-4);

    std::cout << "Test 2 OK: 4 threads" << std::endl;

    mc3.Calculate(res3);

    assert(std::abs(res3.specularReflection - 0.05325) < 1e-4);
    assert(std::abs(res3.diffuseReflection - 0.0175) < 1e-3);
    assert(std::abs(res3.diffuseTransmission - 0.0000549) < 2e-6);
    assert(std::abs(res3.diffuseTransmission + res3.diffuseReflection +
                    res3.specularReflection + res3.absorbed - 1.0) < 1e-4);

    std::cout << "Test 3 OK: single thread" << std::endl;

    MCmultithread<T, Nz, Nr>(sample3, 2e6, 4, sample3.getTotalThickness(), selectedRadius, MTres3);

    assert(std::abs(MTres3.specularReflection - 0.05325) < 1e-4);
    assert(std::abs(MTres3.diffuseReflection - 0.0175) < 1e-3);
    assert(std::abs(MTres3.diffuseTransmission - 0.0000549) < 2e-6);
    assert(std::abs(MTres3.diffuseTransmission + MTres3.diffuseReflection +
                    MTres3.specularReflection + MTres3.absorbed - 1.0) < 1e-4);

    std::cout << "Test 3 OK: 4 threads" << std::endl;

    std::cout << "All tests passed successfully!" << std::endl;
}

void TestsMC::MultiLayerAbsorptionScattering() {

    std::cout << "Glass-Tissue-Glass, absorption & scattering" << std::endl;

    using T = double;
    const int Nz = 1000;
    const int Nr = 10000;
    T selectedRadius = 10e-2;

    Medium<T> tissue1(1.5, 100, 900, 1e-3, 0.9); //MFP == D
    Medium<T> tissue2(1.3, 20, 80, 1e-3, 0.5); //MFP = 10 D
    Medium<T> tissue3(1.6, 700, 300, 10e-3, 0.0); //MFP == 0.1D, it will need more photons

    Medium<T> glass1 (1.6, 0.0, 0.0, 1e-3, 0.0);
    Medium<T> glass2 (1.4, 0.0, 0.0, 1e-3, 0.0);
    Medium<T> glass3 (1.65, 0.0, 0.0, 1e-3, 0.0);

    std::vector<Medium<T>> layer1 = {glass1, tissue1, glass3};
    std::vector<Medium<T>> layer2 = {glass2, tissue2, glass2};
    std::vector<Medium<T>> layer3 = {glass3, tissue3, glass3};

    Sample<T> sample1(layer1, 1.0, 1.0);
    Sample<T> sample2(layer2, 1.0, 1.0);
    Sample<T> sample3(layer3, 1.0, 1.0);

    MonteCarlo<T, Nz, Nr> mc1(sample1, 1e6, sample1.getTotalThickness(), selectedRadius);
    MonteCarlo<T, Nz, Nr> mc2(sample2, 1e6, sample2.getTotalThickness(), selectedRadius);
    MonteCarlo<T, Nz, Nr> mc3(sample3, 1e6, sample3.getTotalThickness(), selectedRadius);

    MCresults<T, Nz, Nr> res1, res2, res3;
    MCresults<T, Nz, Nr> MTres1, MTres2, MTres3;

    mc1.Calculate(res1);

    assert(std::abs(res1.specularReflection - 0.05419) < 1e-4);
    assert(std::abs(res1.diffuseReflection - 0.05813) < 5e-3);
    assert(std::abs(res1.diffuseTransmission - 0.7394) < 1e-3);
    assert(std::abs(res1.diffuseTransmission + res1.diffuseReflection +
                    res1.specularReflection + res1.absorbed - 1.0) < 1e-4);

    std::cout << "Test 1 OK: single thread" << std::endl;

    MCmultithread<T, Nz, Nr>(sample1, 1e6, 4, sample1.getTotalThickness(), selectedRadius, MTres1);

    assert(std::abs(MTres1.specularReflection - 0.05419) < 1e-4);
    assert(std::abs(MTres1.diffuseReflection - 0.05813) < 5e-3);
    assert(std::abs(MTres1.diffuseTransmission - 0.7394) < 1e-3);
    assert(std::abs(MTres1.diffuseTransmission + MTres1.diffuseReflection +
                    MTres1.specularReflection + MTres1.absorbed - 1.0) < 1e-4);

    std::cout << "Test 1 OK: 4 threads" << std::endl;

    mc2.Calculate(res2);

    assert(std::abs(res2.specularReflection - 0.02907) < 1e-4);
    assert(std::abs(res2.diffuseReflection - 0.03695) < 1e-3);
    assert(std::abs(res2.diffuseTransmission - 0.8987) < 1e-3);
    assert(std::abs(res2.diffuseTransmission + res2.diffuseReflection +
                    res2.specularReflection + res2.absorbed - 1.0) < 1e-4);

    std::cout << "Test 2 OK: single thread" << std::endl;

    MCmultithread<T, Nz, Nr>(sample2, 1e6, 4, sample2.getTotalThickness(), selectedRadius, MTres2);

    assert(std::abs(MTres2.specularReflection - 0.02907) < 1e-4);
    assert(std::abs(MTres2.diffuseReflection - 0.03695) < 1e-3);
    assert(std::abs(MTres2.diffuseTransmission - 0.8987) < 1e-3);
    assert(std::abs(MTres2.diffuseTransmission + MTres2.diffuseReflection +
                    MTres2.specularReflection + MTres2.absorbed - 1.0) < 1e-4);

    std::cout << "Test 2 OK: 4 threads" << std::endl;

    mc3.Calculate(res3);

    assert(std::abs(res3.specularReflection - 0.06037) < 1e-4);
    assert(std::abs(res3.diffuseReflection - 0.01718) < 1e-3);
    assert(std::abs(res3.diffuseTransmission - 0.0000541) < 1e-5); // yeah that's a lot
    assert(std::abs(res3.diffuseTransmission + res3.diffuseReflection +
                    res3.specularReflection + res3.absorbed - 1.0) < 1e-4);

    std::cout << "Test 3 OK: single thread" << std::endl;

    MCmultithread<T, Nz, Nr>(sample3, 5e6, 4, sample3.getTotalThickness(), selectedRadius, MTres3);

    assert(std::abs(MTres3.specularReflection - 0.06037) < 1e-4);
    assert(std::abs(MTres3.diffuseReflection - 0.01718) < 1e-3);
    assert(std::abs(MTres3.diffuseTransmission - 0.0000541) < 2e-6);
    assert(std::abs(MTres3.diffuseTransmission + MTres3.diffuseReflection +
                    MTres3.specularReflection + MTres3.absorbed - 1.0) < 1e-4);

    std::cout << "Test 3 OK: 4 threads" << std::endl;

    std::cout << "All tests passed successfully!" << std::endl;

}
