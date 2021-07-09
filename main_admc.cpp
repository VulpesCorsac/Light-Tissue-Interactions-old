// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <random>

#include "MC/Photon.h"
#include "MC/Medium.h"
#include "MC/Fresnel.h"
#include "MC/MonteCarlo.h"
#include "MC/BugerLambert.h"
#include "MC/Sample.h"
#include "MC/Detector.h"
#include "MC/MCmultithread.h"
#include "MC/IMC.h"

#include "AD/NelderMead.h"
#include "AD/Quadrature.h"
#include "AD/RT.h"

#include "Tests/TestIADStandalone.h"

template < typename T, size_t N, bool fix, size_t M, size_t Nz, size_t Nr, bool detector >
void calcAll (T inA, T inT, T inG, T inN, T inD, T inNG, T inDG, bool moveable, int Nthreads, double err) {
    using namespace std;
    auto tissue = Medium<T>::fromAlbedo(inN, inA, inT, inD, inG);
    auto emptyTissue = Medium<T>::fromAlbedo(inN, 0.0, 0.0, inD, 0.0);
    auto glass = Medium<T>::fromCoeffs(inNG, 0.0, 0.0, inDG, 0.0);
    vector<Medium<T>> layers = {glass, tissue, glass};
    Sample<T> mySample(layers, 1.0, 1.0);
    vector<Medium<T>> slides = {glass, glass};
    vector<Medium<T>> emptyLayers = {glass, emptyTissue, glass};
    Sample<T> emptySample (emptyLayers, 1.0, 1.0);

    IntegratingSphere<T> SphereT(0.1, 0.013, 0.013); // dPort2 = zero if the sphere has one port
    IntegratingSphere<T> SphereR(0.1, 0.013, 0.013);

    DetectorDistances<T> distances;
    distances.maxDist  = moveable ? 0.02 : 0.00;
    distances.minDist  = 0.0;
    distances.stepSize = 0.002; // please, enter correct step for your borders

    constexpr int Nphotons = 1e6;
    constexpr T selectedRadius = 10e-2;

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(mySample, Nphotons, Nthreads, mySample.getTotalThickness(), selectedRadius, myResultsMT, SphereR, SphereT, distances);
    cout << myResultsMT << endl;

    static default_random_engine generator(time(0));
    normal_distribution<T> distribution(0.0, err);

    auto rsmeas = myResultsMT.detectedR;
    auto tsmeas = myResultsMT.detectedT;
    T tcmeas = myResultsMT.BugerTransmission;

    vector<pair<T,T>> rSpoilt, tSpoilt;

    ofstream fout;

    T n_slab = tissue.n;
    T n_slide_top, n_slide_bottom;
    if (slides.empty()) {
        n_slide_top = tissue.n;
        n_slide_bottom = tissue.n;
    } else {
        n_slide_top = slides[0].n;
        n_slide_bottom = slides[1].n;
    }

    T aOutIAD, tauOutIAD, gOutIAD; // IAD result will be recorded here
    T aOutIMC, tauOutIMC, gOutIMC; // IMC result will be recorded here
    T checkConvEps = 1e-3;

    if (moveable)
        fout.open(to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + "_" + to_string(err) + "_moveable.txt");
    else
        fout.open(to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + "_" + to_string(err) + "_fixed.txt");
    fout << "a \t tau \t g \t time" << endl;

    for (int i = 0; i < 20; i++) {
        clock_t begin = clock();

        T diff = 0;

        for (auto x : rsmeas) {
            T e1 = distribution(generator);
            rSpoilt.push_back(make_pair(x.first, x.second + e1));
            diff += e1;
        }

        for (auto x : tsmeas){
            T e2 = distribution(generator);
            tSpoilt.push_back(make_pair(x.first, x.second + e2));
            diff += e2;
        }

        T tcSpoilt = tcmeas + distribution(generator);

        cout << diff << endl;

        for (auto x : rSpoilt)
            cout << x.first << " " << x.second << endl;

        for (auto x : tSpoilt)
            cout << x.first << " " << x.second << endl;

        T rStart = rSpoilt[0].second + myResultsMT.specularReflection; //the closest values to total Rs and Ts to be used in IAD algorithm
        T tStart = tSpoilt[0].second + tcSpoilt;

        cout << "rStart = " << rStart << " tStart = " << tStart << " tc = " << tcSpoilt << endl;

        /*
        T rStart = myResultsMT.diffuseReflection + myResultsMT.specularReflection; //the closest values to total Rs and Ts to be used in IAD algorithm
        T tStart = myResultsMT.diffuseTransmission + myResultsMT.BugerTransmission;
        //*/

        IAD<T,M,N,fix>(rStart, tStart, tcSpoilt, n_slab, n_slide_top, n_slide_bottom, aOutIAD, tauOutIAD, gOutIAD);

        cout << "First approximation: Inverse Adding-Doubling" << endl;
        cout << "a = " << aOutIAD << ", tau = " << tauOutIAD << ", g = " << gOutIAD << endl;

        IMC<T,Nz,Nr,detector,N,fix>(rSpoilt, tSpoilt, tcSpoilt, emptyTissue, std::move(slides), Nphotons, Nthreads, emptySample.getTotalThickness(), selectedRadius, SphereR, SphereT, distances, aOutIAD, tauOutIAD, gOutIAD, checkConvEps, aOutIMC, tauOutIMC, gOutIMC);

        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        fout << aOutIMC << "\t" << tauOutIMC << "\t" << gOutIMC << "\t" << elapsed_secs << endl;

        rSpoilt.clear();
        tSpoilt.clear();
    }

    fout.close();
}

int main(int argc, char **argv) {
    TestsIAD::RunAllTests();

    using namespace std;
    using T = double;

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    const int M = 32; // matrix size in Adding-Doubling

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    const bool detector = 1; // spheres => detector = 1; fiber => detector = 0.

    ///CALCULATE ENTRIES FOR MINIMIZATION WITH MONTE-CARLO, OR USE INPUT FILE

    int mode;
    cout << "GRID MODE 0 1" << endl;
    cin >> mode;

    Matrix<T, 1, 6> gridT;
    Matrix<T, 1, 7> gridA, gridG;
    gridT << 0.5, 0.75, 1.0, 1.5, 2.0, 4.0;
    gridA << 0.0001, 0.25, 0.5, 0.75, 0.9, 0.95, 0.999;
    gridG << 0.0001, 0.25, 0.5, 0.75, 0.9, 0.95, 0.999;

    T inA, inT, inG, inN, inD, inNG, inDG;
    if (mode == 0) {
        cout << "Enter tissue and glass parameters: a tau g n d nGlass dGlass" << endl;
        cin >> inA >> inT >> inG >> inN >> inD >> inNG >> inDG;

        bool moveable;
        cout << "Moveable = 1, fixed = 0" << endl;
        cin >> moveable;

        int Nthreads;
        cout << "Enter number of threads" << endl;
        cin >> Nthreads;

        double err;
        cout << "Enter error dispersion" << endl;
        cin >> err;
        calcAll<T, N, fix, M, Nz, Nr, detector>(inA, inT, inG, inN, inD, inNG, inDG, moveable, Nthreads, err);
    } else if (mode == 1) {
        cout << "Enter tissue and glass parameters: n d nGlass dGlass" << endl;
        cin >> inN >> inD >> inNG >> inDG;
        cout << "Enter tissue and glass parameters: a g" << endl;
        cin >> inA >> inG;
        cout << "We'll calculate with these a & g and full grid of tau (0.5 0.75 1.0 1.5 2.0 4.0)" << endl;

        bool moveable;
        cout << "Moveable = 1, fixed = 0" << endl;
        cin >> moveable;

        int Nthreads;
        cout << "Enter number of threads" << endl;
        cin >> Nthreads;

        double err;
        cout << "Enter error dispersion" << endl;
        cin >> err;

        int kt;
        cout << "Enter number of tau to begin with" << endl;
        cin >> kt;
        for (int i = kt; i < 6; i++) {
            inT = gridT(i);
            calcAll<T, N, fix, M, Nz, Nr, detector>(inA, inT, inG, inN, inD, inNG, inDG, moveable, Nthreads, err);
        }
    }

    return 0;
}

