#pragma once

#include "../MC/MonteCarlo.h"
#include "../MC/BugerLambert.h"

using namespace std;

class TestsMC {
public:
    void Tests() {}
    void SingleLayerAbsorptionOnly();
    void MultiLayerAbsorptionOnly();
    void SingleLayerAbsorptionScattering();
    void MultiLayerAbsorptionScattering();
};

constexpr float TOTAL_TOLERANCE = 1e-4;

void TestsMC::SingleLayerAbsorptionOnly() {
    using T = double;

    SphereOnePort<T> SphereT(0.1, 0.01);
    SphereTwoPorts<T> SphereR(0.1, 0.01, 0.01);

    cout << "Single layer, absorption only" << endl;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr T selectedRadius = 10e-2;

    const Medium<T> tissue1(1.5, 1000, 0,  1e-3, 0.9); // MFP == D
    const Medium<T> tissue2(1.3,  100, 0,  1e-3, 0.5); // MFP == 10*D
    const Medium<T> tissue3(1.6, 1000, 0, 10e-3, 0.0); // MFP == 0.1*D, it will need more photons

    const vector<Medium<T>> layer1 = {tissue1};
    const vector<Medium<T>> layer2 = {tissue2};
    const vector<Medium<T>> layer3 = {tissue3};

    const Sample<T> sample1(layer1, 1.0, 1.0);
    const Sample<T> sample2(layer2, 1.0, 1.0);
    const Sample<T> sample3(layer3, 1.0, 1.0);

    MonteCarlo<T, Nz, Nr> mc1(sample1, 1e6, sample1.getTotalThickness(), selectedRadius, SphereR, SphereT);
    MonteCarlo<T, Nz, Nr> mc2(sample2, 1e6, sample2.getTotalThickness(), selectedRadius, SphereR, SphereT);
    MonteCarlo<T, Nz, Nr> mc3(sample3, 5e6, sample3.getTotalThickness(), selectedRadius, SphereR, SphereT);

    MCresults<T, Nz, Nr> res1, res2, res3;
    MCresults<T, Nz, Nr> MTres1, MTres2, MTres3;

    mc1.Calculate(res1);
    assert(abs(res1.specularReflection - 0.04) < 1e-4);
    assert(abs(res1.diffuseReflection - 0.004989) < 1e-3);
    assert(abs(res1.diffuseTransmission - 0.3390) < 2e-3);
    assert(abs(res1.diffuseTransmission - BugerLambert(1.0, 1.5, 1.0, 1.0)) < 2e-3);
    assert(abs(res1.diffuseTransmission + res1.diffuseReflection + res1.specularReflection + res1.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 1 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample1, 1e6, 4, sample1.getTotalThickness(), selectedRadius, MTres1);
    assert(abs(MTres1.specularReflection - 0.04) < 1e-4);
    assert(abs(MTres1.diffuseReflection - 0.004989) < 1e-3);
    assert(abs(MTres1.diffuseTransmission - 0.3390) < 2e-3);
    assert(abs(MTres1.diffuseTransmission - BugerLambert(1.0, 1.5, 1.0, 1.0)) < 2e-3);
    assert(abs(MTres1.diffuseTransmission + MTres1.diffuseReflection + MTres1.specularReflection + MTres1.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 1 OK: 4 threads" << endl;

    mc2.Calculate(res2);
    assert(abs(res2.specularReflection - 0.0170132) < 1e-4);
    assert(abs(res2.diffuseReflection - 0.01346) < 1e-3);
    assert(abs(res2.diffuseTransmission - 0.8743) < 2e-3);
    assert(abs(res2.diffuseTransmission - BugerLambert(0.1, 1.3, 1.0, 1.0)) < 1e-3);
    assert(abs(res2.diffuseTransmission + res2.diffuseReflection + res2.specularReflection + res2.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 2 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample2, 1e6, 4, sample2.getTotalThickness(), selectedRadius, MTres2);
    assert(abs(MTres2.specularReflection - 0.0170132) < 1e-4);
    assert(abs(MTres2.diffuseReflection - 0.01346) < 1e-3);
    assert(abs(MTres2.diffuseTransmission - 0.8743) < 2e-3);
    assert(abs(MTres2.diffuseTransmission - BugerLambert(0.1, 1.3, 1.0, 1.0)) < 1e-3);
    assert(abs(MTres2.diffuseTransmission + MTres2.diffuseReflection + MTres2.specularReflection + MTres2.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 2 OK: 4 threads" << endl;

    mc3.Calculate(res3);
    assert(abs(res3.specularReflection - 0.05325) < 1e-4);
    assert(abs(res3.diffuseReflection - 0.0) < 1e-3);
    assert(abs(res3.diffuseTransmission - 0.00004069) < 8e-6);
    assert(abs(res3.diffuseTransmission - BugerLambert(10.0, 1.6, 1.0, 1.0)) < 8e-6);
    assert(abs(res3.diffuseTransmission + res3.diffuseReflection + res3.specularReflection + res3.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 3 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample3, 2e7, 4, sample3.getTotalThickness(), selectedRadius, MTres3);
    assert(abs(MTres3.specularReflection - 0.05325) < 1e-4);
    assert(abs(MTres3.diffuseReflection - 0.0) < 1e-3);
    assert(abs(MTres3.diffuseTransmission - 0.00004069) < 5e-6);
    assert(abs(MTres3.diffuseTransmission - BugerLambert(10.0, 1.6, 1.0, 1.0)) < 4e-6);
    assert(abs(MTres3.diffuseTransmission + MTres3.diffuseReflection + MTres3.specularReflection + MTres3.absorbed - 1.0) < 1e-4);
    cout << "Test 3 OK: 4 threads" << endl;

    cout << "All tests passed successfully!" << endl;
}

void TestsMC::MultiLayerAbsorptionOnly() {
    using T = double;

    SphereOnePort<T> SphereT(0.1, 0.01);
    SphereTwoPorts<T> SphereR(0.1, 0.01, 0.01);

    cout << "Glass-tissue-glass, absorption only" << endl;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr T selectedRadius = 10e-2;

    const Medium<T> tissue1(1.5, 1000, 0,  1e-3, 0.9); // MFP == D
    const Medium<T> tissue2(1.3,  100, 0,  1e-3, 0.5); // MFP == 10*D
    const Medium<T> tissue3(1.6, 1000, 0, 10e-3, 0.0); // MFP == 0.1*D, it will need more photons

    const Medium<T> glass1 (1.6 , 0.0, 0.0, 1e-3, 0.0);
    const Medium<T> glass2 (1.4 , 0.0, 0.0, 1e-3, 0.0);
    const Medium<T> glass3 (1.65, 0.0, 0.0, 1e-3, 0.0);

    const vector<Medium<T>> layer1 = {glass1, tissue1, glass1};
    const vector<Medium<T>> layer2 = {glass2, tissue2, glass2};
    const vector<Medium<T>> layer3 = {glass3, tissue3, glass3};

    const Sample<T> sample1(layer1, 1.0, 1.0);
    const Sample<T> sample2(layer2, 1.0, 1.0);
    const Sample<T> sample3(layer3, 1.0, 1.0);

    MonteCarlo<T, Nz, Nr> mc1(sample1, 1e6, sample1.getTotalThickness(), selectedRadius, SphereR, SphereT);
    MonteCarlo<T, Nz, Nr> mc2(sample2, 1e6, sample2.getTotalThickness(), selectedRadius, SphereR, SphereT);
    MonteCarlo<T, Nz, Nr> mc3(sample3, 5e6, sample3.getTotalThickness(), selectedRadius, SphereR, SphereT);

    MCresults<T, Nz, Nr> res1, res2, res3;
    MCresults<T, Nz, Nr> MTres1, MTres2, MTres3;

    mc1.Calculate(res1);
    assert(abs(res1.specularReflection - 0.05419) < 1e-4);
    assert(abs(res1.diffuseReflection - 0.00656) < 1e-3);
    assert(abs(res1.diffuseTransmission - 0.3292) < 2e-3);
    assert(abs(res1.diffuseTransmission - BugerLambert(1.0, 1.5, 1.6, 1.6)) < 2e-3);
    assert(abs(res1.diffuseTransmission + res1.diffuseReflection + res1.specularReflection + res1.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 1 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample1, 1e6, 4, sample1.getTotalThickness(), selectedRadius, MTres1);
    assert(abs(MTres1.specularReflection - 0.05419) < 1e-4);
    assert(abs(MTres1.diffuseReflection - 0.00656) < 1e-3);
    assert(abs(MTres1.diffuseTransmission - 0.3292) < 2e-3);
    assert(abs(MTres1.diffuseTransmission - BugerLambert(1.0, 1.5, 1.6, 1.6)) < 2e-3);
    assert(abs(MTres1.diffuseTransmission + MTres1.diffuseReflection + MTres1.specularReflection + MTres1.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 1 OK: 4 threads" << endl;

    mc2.Calculate(res2);
    assert(abs(res2.specularReflection - 0.02907) < 1e-4);
    assert(abs(res2.diffuseReflection - 0.02244) < 1e-3);
    assert(abs(res2.diffuseTransmission - 0.853) < 2e-3);
    assert(abs(res2.diffuseTransmission - BugerLambert(0.1, 1.3, 1.4, 1.4)) < 1e-3);
    assert(abs(res2.diffuseTransmission + res2.diffuseReflection + res2.specularReflection + res2.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 2 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample2, 1e6, 4, sample2.getTotalThickness(), selectedRadius, MTres2);
    assert(abs(MTres2.specularReflection - 0.02907) < 1e-4);
    assert(abs(MTres2.diffuseReflection - 0.02244) < 1e-3);
    assert(abs(MTres2.diffuseTransmission - 0.853) < 2e-3);
    assert(abs(MTres2.diffuseTransmission - BugerLambert(0.1, 1.3, 1.4, 1.4)) < 1e-3);
    assert(abs(MTres2.diffuseTransmission + MTres2.diffuseReflection + MTres2.specularReflection + MTres2.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 2 OK: 4 threads" << endl;

    mc3.Calculate(res3);
    assert(abs(res3.specularReflection - 0.06037) < 1e-4);
    assert(abs(res3.diffuseReflection - 0.0) < 3e-3);
    assert(abs(res3.diffuseTransmission - 0.00004008) < 8e-6);
    assert(abs(res3.diffuseTransmission - BugerLambert(10.0, 1.6, 1.65, 1.65)) < 8e-6);
    assert(abs(res3.diffuseTransmission + res3.diffuseReflection + res3.specularReflection + res3.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 3 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample3, 20e6, 4, sample3.getTotalThickness(), selectedRadius, MTres3);
    assert(abs(MTres3.specularReflection - 0.06037) < 1e-4);
    assert(abs(MTres3.diffuseReflection - 0.0) < 1e-3);
    assert(abs(MTres3.diffuseTransmission - 0.00004008) < 4e-6);
    assert(abs(MTres3.diffuseTransmission - BugerLambert(10.0, 1.6, 1.65, 1.65)) < 4e-6);
    assert(abs(MTres3.diffuseTransmission + MTres3.diffuseReflection + MTres3.specularReflection + MTres3.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 3 OK: 4 threads" << endl;

    cout << "All tests passed successfully!" << endl;
}

void TestsMC::SingleLayerAbsorptionScattering() {
    using T = double;

    SphereOnePort<T> SphereT(0.1, 0.01);
    SphereTwoPorts<T> SphereR(0.1, 0.01, 0.01);

    cout << "Single layer, absorption & scattering" << endl;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr T selectedRadius = 10e-2;

    const Medium<T> tissue1(1.5, 100, 900,  1e-3, 0.9); // MFP == D
    const Medium<T> tissue2(1.3,  20,  80,  1e-3, 0.5); // MFP == 10*D
    const Medium<T> tissue3(1.6, 700, 300, 10e-3, 0.0); // MFP == 0.1*D, it will need more photons

    const vector<Medium<T>> layer1 = {tissue1};
    const vector<Medium<T>> layer2 = {tissue2};
    const vector<Medium<T>> layer3 = {tissue3};

    const Sample<T> sample1(layer1, 1.0, 1.0);
    const Sample<T> sample2(layer2, 1.0, 1.0);
    const Sample<T> sample3(layer3, 1.0, 1.0);

    MonteCarlo<T, Nz, Nr> mc1(sample1, 5e5, sample1.getTotalThickness(), selectedRadius, SphereR, SphereT);
    MonteCarlo<T, Nz, Nr> mc2(sample2, 5e5, sample2.getTotalThickness(), selectedRadius, SphereR, SphereT);
    MonteCarlo<T, Nz, Nr> mc3(sample3, 1e6, sample3.getTotalThickness(), selectedRadius, SphereR, SphereT);

    MCresults<T, Nz, Nr> res1, res2, res3;
    MCresults<T, Nz, Nr> MTres1, MTres2, MTres3;

    mc1.Calculate(res1);
    assert(abs(res1.specularReflection - 0.04) < 1e-4);
    assert(abs(res1.diffuseReflection - 0.0435) < 1e-3);
    assert(abs(res1.diffuseTransmission - 0.767) < 2e-3);
    assert(abs(res1.diffuseTransmission + res1.diffuseReflection + res1.specularReflection + res1.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 1 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample1, 5e5, 4, sample1.getTotalThickness(), selectedRadius, MTres1);
    assert(abs(MTres1.specularReflection - 0.04) < 1e-4);
    assert(abs(MTres1.diffuseReflection - 0.0435) < 1e-3);
    assert(abs(MTres1.diffuseTransmission - 0.767) < 2e-3);
    assert(abs(MTres1.diffuseTransmission + MTres1.diffuseReflection + MTres1.specularReflection + MTres1.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 1 OK: 4 threads" << endl;

    mc2.Calculate(res2);
    assert(abs(res2.specularReflection - 0.01701) < 1e-4);
    assert(abs(res2.diffuseReflection - 0.0272) < 1e-3);
    assert(abs(res2.diffuseTransmission - 0.9206) < 1e-3);
    assert(abs(res2.diffuseTransmission + res2.diffuseReflection + res2.specularReflection + res2.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 2 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample2, 5e5, 4, sample2.getTotalThickness(), selectedRadius, MTres2);
    assert(abs(MTres2.specularReflection - 0.01701) < 1e-4);
    assert(abs(MTres2.diffuseReflection - 0.0272) < 1e-3);
    assert(abs(MTres2.diffuseTransmission - 0.9206) < 1e-3);
    assert(abs(MTres2.diffuseTransmission + MTres2.diffuseReflection + MTres2.specularReflection + MTres2.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 2 OK: 4 threads" << endl;

    mc3.Calculate(res3);
    assert(abs(res3.specularReflection - 0.05325) < 1e-4);
    assert(abs(res3.diffuseReflection - 0.0175) < 1e-3);
    assert(abs(res3.diffuseTransmission - 0.0000549) < 2e-5);
    assert(abs(res3.diffuseTransmission + res3.diffuseReflection + res3.specularReflection + res3.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 3 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample3, 2e6, 4, sample3.getTotalThickness(), selectedRadius, MTres3);
    assert(abs(MTres3.specularReflection - 0.05325) < 2e-4);
    assert(abs(MTres3.diffuseReflection - 0.0175) < 1e-3);
    assert(abs(MTres3.diffuseTransmission - 0.0000549) < 2e-5);
    assert(abs(MTres3.diffuseTransmission + MTres3.diffuseReflection + MTres3.specularReflection + MTres3.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 3 OK: 4 threads" << endl;

    cout << "All tests passed successfully!" << endl;
}

void TestsMC::MultiLayerAbsorptionScattering() {
    using T = double;

    SphereOnePort<T> SphereT(0.1, 0.01);
    SphereTwoPorts<T> SphereR(0.1, 0.01, 0.01);

    cout << "Glass-Tissue-Glass, absorption & scattering" << endl;

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr T selectedRadius = 10e-2;

    const Medium<T> tissue1(1.5, 100, 900,  1e-3, 0.9); // MFP == D
    const Medium<T> tissue2(1.3,  20,  80,  1e-3, 0.5); // MFP == 10*D
    const Medium<T> tissue3(1.6, 700, 300, 10e-3, 0.0); // MFP == 0.1*D, it will need more photons

    const Medium<T> glass1 (1.6 , 0.0, 0.0, 1e-3, 0.0);
    const Medium<T> glass2 (1.4 , 0.0, 0.0, 1e-3, 0.0);
    const Medium<T> glass3 (1.65, 0.0, 0.0, 1e-3, 0.0);

    const vector<Medium<T>> layer1 = {glass1, tissue1, glass3}; /// TODO: recalculate for glass 1
    const vector<Medium<T>> layer2 = {glass2, tissue2, glass2};
    const vector<Medium<T>> layer3 = {glass3, tissue3, glass3};

    const Sample<T> sample1(layer1, 1.0, 1.0);
    const Sample<T> sample2(layer2, 1.0, 1.0);
    const Sample<T> sample3(layer3, 1.0, 1.0);

    MonteCarlo<T, Nz, Nr> mc1(sample1, 1e6, sample1.getTotalThickness(), selectedRadius, SphereR, SphereT);
    MonteCarlo<T, Nz, Nr> mc2(sample2, 1e6, sample2.getTotalThickness(), selectedRadius, SphereR, SphereT);
    MonteCarlo<T, Nz, Nr> mc3(sample3, 1e6, sample3.getTotalThickness(), selectedRadius, SphereR, SphereT);

    MCresults<T, Nz, Nr> res1, res2, res3;
    MCresults<T, Nz, Nr> MTres1, MTres2, MTres3;

    mc1.Calculate(res1);
    assert(abs(res1.specularReflection - 0.05419) < 1e-4);
    assert(abs(res1.diffuseReflection - 0.05813) < 5e-3);
    assert(abs(res1.diffuseTransmission - 0.7394) < 1e-3);
    assert(abs(res1.diffuseTransmission + res1.diffuseReflection + res1.specularReflection + res1.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 1 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample1, 1e6, 4, sample1.getTotalThickness(), selectedRadius, MTres1);
    assert(abs(MTres1.specularReflection - 0.05419) < 1e-4);
    assert(abs(MTres1.diffuseReflection - 0.05813) < 5e-3);
    assert(abs(MTres1.diffuseTransmission - 0.7394) < 2e-3);
    assert(abs(MTres1.diffuseTransmission + MTres1.diffuseReflection + MTres1.specularReflection + MTres1.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 1 OK: 4 threads" << endl;

    mc2.Calculate(res2);
    assert(abs(res2.specularReflection - 0.02907) < 1e-4);
    assert(abs(res2.diffuseReflection - 0.03695) < 1e-3);
    assert(abs(res2.diffuseTransmission - 0.8987) < 1e-3);
    assert(abs(res2.diffuseTransmission + res2.diffuseReflection + res2.specularReflection + res2.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 2 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample2, 1e6, 4, sample2.getTotalThickness(), selectedRadius, MTres2);
    assert(abs(MTres2.specularReflection - 0.02907) < 1e-4);
    assert(abs(MTres2.diffuseReflection - 0.03695) < 1e-3);
    assert(abs(MTres2.diffuseTransmission - 0.8987) < 1e-3);
    assert(abs(MTres2.diffuseTransmission + MTres2.diffuseReflection + MTres2.specularReflection + MTres2.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 2 OK: 4 threads" << endl;

    mc3.Calculate(res3);
    assert(abs(res3.specularReflection - 0.06037) < 1e-4);
    assert(abs(res3.diffuseReflection - 0.01718) < 1e-3);
    assert(abs(res3.diffuseTransmission - 0.0000541) < 2e-5); // yeah that's a lot
    assert(abs(res3.diffuseTransmission + res3.diffuseReflection + res3.specularReflection + res3.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 3 OK: single thread" << endl;

    MCmultithread<T, Nz, Nr>(sample3, 10e6, 4, sample3.getTotalThickness(), selectedRadius, MTres3);
    assert(abs(MTres3.specularReflection - 0.06037) < 1e-4);
    assert(abs(MTres3.diffuseReflection - 0.01718) < 1e-3);
    assert(abs(MTres3.diffuseTransmission - 0.0000541) < 5e-6);
    assert(abs(MTres3.diffuseTransmission + MTres3.diffuseReflection + MTres3.specularReflection + MTres3.absorbed - 1.0) < TOTAL_TOLERANCE);
    cout << "Test 3 OK: 4 threads" << endl;

    cout << "All tests passed successfully!" << endl;
}
