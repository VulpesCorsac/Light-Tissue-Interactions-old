#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

int main(int argc, char **argv) {
    using namespace std;
    using T = double;

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    const int M = 32; // matrix size in Adding-Doubling

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr int Nphotons = 1e6;
    constexpr T selectedRadius = 10e-2;

    const bool detector = 1; // spheres => detector = 1; fiber => detector = 0.
    IntegratingSphere<T> SphereT(0.1, 0.013, 0.013); // dPort2 = zero if the sphere has one port
    IntegratingSphere<T> SphereR(0.1, 0.013, 0.013);

    DetectorDistances<T> distances;
    distances.maxDist = 0.2;
    distances.minDist = 0.0;
    distances.stepSize = 0.02; // please, enter correct step for your borders

    ///CALCULATE ENTRIES FOR MINIMIZATION WITH MONTE-CARLO, OR USE INPUT FILE
    auto tissue = Medium<T>::fromAlbedo(1.5, 0.9, 1.0, 1e-3, 0.9);
    auto emptyTissue = Medium<T>::fromAlbedo(1.5, 0.0, 0.0, 1e-3, 0.0);
    auto glass = Medium<T>::fromCoeffs(1.6, 0.0, 0.0, 1e-3, 0.0);
    vector<Medium<T>> layers = {glass, tissue, glass};
    Sample<T> mySample(layers, 1.0, 1.0);
    vector<Medium<T>> slides = {glass, glass};
    vector<Medium<T>> emptyLayers = {glass, emptyTissue, glass};
    Sample<T> emptySample (emptyLayers, 1.0, 1.0);

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(mySample, Nphotons, 4, mySample.getTotalThickness(), selectedRadius, myResultsMT, SphereR, SphereT, distances);
    cout << myResultsMT << endl;

    auto rsmeas = myResultsMT.detectedR;
    auto tsmeas = myResultsMT.detectedT;
    T tcmeas = myResultsMT.BugerTransmission;
    T rStart = rsmeas[0].second + myResultsMT.specularReflection; //the closest values to total Rs and Ts to be used in IAD algorithm
    T tStart = tsmeas[0].second + tcmeas;

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

    IAD<T,M,N,fix>(rStart, tStart, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOutIAD, tauOutIAD, gOutIAD);

    cout << "First approximation: Inverse Adding-Doubling" << endl;
    cout << "a = " << aOutIAD << ", tau = " << tauOutIAD << ", g = " << gOutIAD << endl;

    T aOutIMC, tauOutIMC, gOutIMC; // IMC result will be recorded here

    IMC<T,Nz,Nr,detector,N,fix>(rsmeas, tsmeas, tcmeas, emptyTissue, std::move(slides), Nphotons, 4, emptySample.getTotalThickness(), selectedRadius, SphereR, SphereT, distances, aOutIAD, tauOutIAD, gOutIAD, aOutIMC, tauOutIMC, gOutIMC);

    return 0;
}

