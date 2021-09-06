// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MC/Photon.h"
#include "MC/Medium.h"
#include "MC/Fresnel.h"
#include "MC/MonteCarlo.h"
#include "MC/BugerLambert.h"
#include "MC/Sample.h"
#include "MC/Detector.h"
#include "MC/MCmultithread.h"
#include "MC/IMC.h"
#include "Utils/ReadFiles.h"

#include "AD/NelderMead.h"
#include "AD/Quadrature.h"
#include "AD/RT.h"

//#include "Tests/TestIADStandalone.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <random>

template < typename T, size_t N, bool fix, size_t M, size_t Nz, size_t Nr, bool detector >
void calcAll(T inA, T inT, T inG, T inN, T inD, T inNG, T inDG, bool moveable, int Nthreads, double err) {
    using namespace std;

    const auto tissue = Medium<T>::fromAlbedo(inN, inA, inT, inD, inG);
    const auto emptyTissue = Medium<T>::fromAlbedo(inN, 0.0, 0.0, inD, 0.0);
    const auto glass = Medium<T>::fromCoeffs(inNG, 0.0, 0.0, inDG, 0.0);
    vector<Medium<T>> layers = {glass, tissue, glass};
    Sample<T> mySample(layers, 1.0, 1.0);
    vector<Medium<T>> slides = {glass, glass};
    vector<Medium<T>> emptyLayers = {glass, emptyTissue, glass};
    Sample<T> emptySample (emptyLayers, 1.0, 1.0);

    IntegratingSphere<T> SphereT(0.1, 0.013, 0.013); // dPort2 = zero if the sphere has one port
    IntegratingSphere<T> SphereR(0.1, 0.013, 0.013);

    DetectorDistances<T> distances;
    distances.maxDist  = moveable ? 0.04 : 0.00;
    distances.minDist  = 0;
    distances.stepSize = 0.002; // please, enter correct step for your borders

    constexpr int Nphotons = 1E6;
    constexpr T selectedRadius = 10E-2;

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(mySample, Nphotons, Nthreads, mySample.getTotalThickness(), selectedRadius, myResultsMT, SphereR, SphereT, distances);
    cout << myResultsMT << endl;

    /// TODO: Utils\Random.h
    static default_random_engine generator(time(0));
    normal_distribution<T> distribution(0, err);

    const auto rsmeas = myResultsMT.detectedR;
    const auto tsmeas = myResultsMT.detectedT;
    T tcmeas = myResultsMT.BugerTransmission;

    ofstream Rdfile;
    ofstream Tdfile;
    ofstream Tcfile;

    Rdfile.open("Output files/Rd_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");
    Tdfile.open("Output files/Td_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");
    Tcfile.open("Output files/Tc_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");

    /// TODO: throw exception if can't open. Anyway you try to do something with the file.
    if (!Rdfile.is_open())
        cout << "Failed to open file Rd" << endl;
    if (!Tdfile.is_open())
        cout << "Failed to open file Td" << endl;
    if (!Tcfile.is_open())
        cout << "Failed to open file Tc" << endl;

    for (const auto& x: rsmeas)
        Rdfile << x.first << "\t" << x.second << '\n';
    for (const auto& x: tsmeas)
        Tdfile << x.first << "\t" << x.second << '\n';
    Tcfile << 0.000 << "\t" << tcmeas << '\n';

    Rdfile.close();
    Tdfile.close();
    Tcfile.close();

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
    T checkConvEps = 1E-3;

    if (moveable)
        fout.open("Output files/" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + "_" + to_string(err) + "_moveable.txt");
    else
        fout.open("Output files/" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + "_" + to_string(err) + "_fixed.txt");
    fout << "a \t tau \t g \t time" << '\n';

    for (int i = 0; i < 20; i++) {
        clock_t begin = clock();

        T diff = 0;

        for (const auto& x: rsmeas) {
            T e1 = distribution(generator);
            rSpoilt.push_back(make_pair(x.first, x.second + e1));
            diff += e1;
        }

        for (const auto& x: tsmeas){
            T e2 = distribution(generator);
            tSpoilt.push_back(make_pair(x.first, x.second + e2));
            diff += e2;
        }

        T tcSpoilt = tcmeas + distribution(generator);

        cout << diff << endl;

        for (const auto& x: rSpoilt)
            cout << x.first << " " << x.second << endl;

        for (const auto& x: tSpoilt)
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

template < typename T, size_t N, bool fix, size_t M, size_t Nz, size_t Nr, bool detector >
void calcForward (T inA, T inT, T inG, T inN, T inD, T inNG, T inDG, bool moveable, int Nthreads, double err) {
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
    distances.maxDist  = moveable ? 0.04 : 0.00;
    distances.minDist  = 0;
    distances.stepSize = 0.002; // please, enter correct step for your borders

    constexpr int Nphotons = 1E6;
    constexpr T selectedRadius = 10E-2;

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(mySample, Nphotons, Nthreads, mySample.getTotalThickness(), selectedRadius, myResultsMT, SphereR, SphereT, distances);
    cout << myResultsMT << endl;

    static default_random_engine generator(time(0));
    normal_distribution<T> distribution(0, err);

    auto rsmeas = myResultsMT.detectedR;
    auto tsmeas = myResultsMT.detectedT;
    T tcmeas = myResultsMT.BugerTransmission;

    ofstream Rdfile;
    ofstream Tdfile;
    ofstream Tcfile;

    Rdfile.open("Output files/Rd_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");
    Tdfile.open("Output files/Td_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");
    Tcfile.open("Output files/Tc_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");

    /// TODO: throw exception if can't open. Anyway you try to do something with the file.
    if (!Rdfile.is_open())
        cout << "Failed to open file Rd" << endl;
    if (!Tdfile.is_open())
        cout << "Failed to open file Td" << endl;
    if (!Tcfile.is_open())
        cout << "Failed to open file Tc" << endl;

    for (const auto& x: rsmeas)
        Rdfile << x.first << "\t" << x.second << '\n';
    for (const auto& x: tsmeas)
        Tdfile << x.first << "\t" << x.second << '\n';
    Tcfile << 0.000 << "\t" << tcmeas << '\n';

    Rdfile.close();
    Tdfile.close();
    Tcfile.close();
}

template < typename T, size_t N, bool fix, size_t M, size_t Nz, size_t Nr, bool detector >
void calcInverse (const string& settingsFile, int Nthreads) {
    using namespace std;

    /*
    auto tissue = Medium<T>::fromAlbedo(inN, inA, inT, inD, inG);
    auto emptyTissue = Medium<T>::fromAlbedo(inN, 0.0, 0.0, inD, 0.0);
    auto glass = Medium<T>::fromCoeffs(inNG, 0.0, 0.0, inDG, 0.0);
    vector<Medium<T>> layers = {glass, tissue, glass};
    Sample<T> mySample(layers, 1.0, 1.0);
    vector<Medium<T>> slides = {glass, glass};
    vector<Medium<T>> emptyLayers = {glass, emptyTissue, glass};
    Sample<T> emptySample (emptyLayers, 1.0, 1.0);

    T n_slab = tissue.n;
    T n_slide_top, n_slide_bottom;
    if (slides.empty()) {
        n_slide_top = tissue.n;
        n_slide_bottom = tissue.n;
    } else {
        n_slide_top = slides[0].n;
        n_slide_bottom = slides[1].n;
    }
    //*/

    int Nphotons;
    constexpr T selectedRadius = 10E-2;

    Sample<T> emptySample;
    IntegratingSphere<T> SphereR, SphereT;
    bool moveable;
    vector<pair<T,T>> Rd, Td, Tc;
    readSettings<T>(settingsFile, emptySample, SphereR, SphereT, moveable, Nphotons, Rd, Td, Tc);

    DetectorDistances<T> distances;
    distances.maxDist  = T(Rd[Rd.size()-1].first);
    distances.minDist  = T(Rd[0].first);
    distances.stepSize = T(Rd[Rd.size()-1].first - Rd[Rd.size() - 2].first); // please, enter correct step for your borders

    Medium<T> emptyTissue = Medium<T>::fromAlbedo(0.0, 0.0, 0.0, 0.0, 0.0);
    T rSpec, n_slab, n_slide_top, n_slide_bottom;
    vector<Medium<T>> slides = {};

    if (emptySample.getNlayers() == 1) {
        rSpec = FresnelR(emptySample.getNvacUpper(), emptySample.getMedium(0).n, 1.0);
        emptyTissue = emptySample.getMedium(0);
        n_slab = emptyTissue.n;
        n_slide_top = emptyTissue.n;
        n_slide_bottom = emptyTissue.n;
    } else {
        slides = {emptySample.getMedium(0), emptySample.getMedium(emptySample.getNlayers() - 1)};
        T r1 = FresnelR(emptySample.getNvacUpper(), emptySample.getMedium(0).n, 1.0);
        T r2 = FresnelR(emptySample.getMedium(0).n, emptySample.getMedium(1).n, 1.0);
        rSpec = (r1 + r2 - 2 * r1 * r2) / (1 - r1 * r2);
        emptyTissue = emptySample.getMedium(1); //yeah i need to make several layers possible
        n_slab = emptyTissue.n;
        n_slide_top = slides[0].n;
        n_slide_bottom = slides[1].n;
    }

    T aOutIAD, tauOutIAD, gOutIAD; // IAD result will be recorded here
    T aOutIMC, tauOutIMC, gOutIMC; // IMC result will be recorded here
    T checkConvEps = 1E-3;

    /*
    cout << "Enter starting points R T" << endl;
    T inRstart, inTstart;
    cin >> inRstart >> inTstart;
    T rStart = inRstart + rSpec; //the closest values to total Rs and Ts to be used in IAD algorithm
    T tStart = inTstart + Tc[0].second;
    //*/

    T rStart = Rd[0].second + rSpec; //the closest values to total Rs and Ts to be used in IAD algorithm
    T tStart = Td[0].second + Tc[0].second;

    IAD<T,M,N,fix>(rStart, tStart, Tc[0].second, n_slab, n_slide_top, n_slide_bottom, aOutIAD, tauOutIAD, gOutIAD);
    cout << "First approximation: Inverse Adding-Doubling" << endl;
    cout << "a = " << aOutIAD << ", tau = " << tauOutIAD << ", g = " << gOutIAD << endl;

    IMC<T,Nz,Nr,detector,N,fix>(Rd, Td, Tc[0].second, emptyTissue, std::move(slides), Nphotons, Nthreads, emptySample.getTotalThickness(), selectedRadius, SphereR, SphereT, distances, aOutIAD, tauOutIAD, gOutIAD, checkConvEps, aOutIMC, tauOutIMC, gOutIMC);
    cout << "Absorption coefficient ua = " << tauOutIMC * (1 - aOutIMC) / emptyTissue.D << endl;
    cout << "Scattering coefficient us = " << tauOutIMC * aOutIMC / emptyTissue.D << endl;
    cout << "Scattering anisotropy g = " << gOutIMC << endl;


    auto tissueFin = Medium<T>::fromAlbedo(n_slab, aOutIMC, tauOutIMC, emptyTissue.D, gOutIMC);
    vector<Medium<T>> layersFin;
    if (emptySample.getNlayers() == 1)
        layersFin = {tissueFin};
    else
        layersFin = {emptySample.getMedium(0), tissueFin, emptySample.getMedium(emptySample.getNlayers() - 1)};
    Sample<T> mySampleFin(layersFin, 1.0, 1.0);

    MCresults<T,Nz,Nr,detector> myResultsMTfin;
    MCmultithread<T,Nz,Nr,detector>(mySampleFin, Nphotons, Nthreads, mySampleFin.getTotalThickness(), selectedRadius, myResultsMTfin, SphereR, SphereT, distances);
    cout << myResultsMTfin << endl;

    auto RdFin = myResultsMTfin.detectedR;
    auto TdFin = myResultsMTfin.detectedT;
    T TcFin = myResultsMTfin.BugerTransmission;

    ofstream Rdfile;
    ofstream Tdfile;
    ofstream Tcfile;

    Rdfile.open("Rd_" + to_string(aOutIMC) + "_" + to_string(tauOutIMC) + "_" + to_string(gOutIMC) + ".txt");
    Tdfile.open("Td_" + to_string(aOutIMC) + "_" + to_string(tauOutIMC) + "_" + to_string(gOutIMC) + ".txt");
    Tcfile.open("Tc_" + to_string(aOutIMC) + "_" + to_string(tauOutIMC) + "_" + to_string(gOutIMC) + ".txt");

    /// TODO: throw exception if can't open. Anyway you try to do something with the file.
    if (!Rdfile.is_open())
        cout << "Failed to open file Rd" << endl;
    if (!Tdfile.is_open())
        cout << "Failed to open file Td" << endl;
    if (!Tcfile.is_open())
        cout << "Failed to open file Tc" << endl;

    for (const auto& x: RdFin)
        Rdfile << x.first << "\t" << x.second << '\n';
    for (const auto& x: TdFin)
        Tdfile << x.first << "\t" << x.second << '\n';
    Tcfile << 0.000 << "\t" << TcFin << '\n';

    Rdfile.close();
    Tdfile.close();
    Tcfile.close();
}

int main(int argc, char **argv) {
    // TestsIAD::RunAllTests();

    using namespace std;
    using T = double; //

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    const int M = 32; // matrix size in Adding-Doubling

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    const bool detector = 1; // spheres => detector = 1; fiber => detector = 0.

    ///CALCULATE ENTRIES FOR MINIMIZATION WITH MONTE-CARLO, OR USE INPUT FILE

    bool developerMode;
    cout << "SELECT MODE" << endl;
    cout << "MODE 0: USER MODE (recommended)" << endl;
    cout << "MODE 1: DEVELOPER MODE" << endl;
    cin >> developerMode;

    if (developerMode == 0) {
        cout << "Note that the first entries in diffuse reflection and transmission files will be used to estimate the initial point of MCML algorithm" << endl;
        int Nthreads;
        cout << "Enter number of threads" << endl;
        cin >> Nthreads;
        string settingsFname0 = "Settings & input files/SETTINGS.txt";
        calcInverse<T, N, fix, M, Nz, Nr, detector>(settingsFname0, Nthreads);
    } else {
        int mode;
        cout << "SELECT MODE 0 1 2 3" << endl;
        cout << "MODE 0: Forward + inverse problem for fixed/moveable spheres and one set of parameters" << endl;
        cout << "MODE 1: Forward + inverse problem for fixed/moveable spheres and several sets of parameters (grid of taus 0.5 0.75 1.0 1.5 2.0 4.0)" << endl;
        cout << "MODE 2: find optical properties from settings file" << endl;
        cout << "MODE 3: R(z) and T(z) for fixed/moveable spheres and one set of parameters" << endl;
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
        } else if (mode == 2) {
            cout << "Note that the first entries in diffuse reflection and transmission files will be used to estimate the initial point of MCML algorithm" << endl;
            int Nthreads;
            cout << "Enter number of threads" << endl;
            cin >> Nthreads;
            string settingsFname1 = "Settings & input files/SETTINGS.txt";
            calcInverse<T, N, fix, M, Nz, Nr, detector>(settingsFname1, Nthreads);

        } else if (mode == 3) {
            cout << "Enter tissue and glass parameters: a tau g n d nGlass dGlass" << endl;
            cin >> inA >> inT >> inG >> inN >> inD >> inNG >> inDG;

            bool moveable;
            cout << "Moveable = 1, fixed = 0" << endl;
            cin >> moveable;

            int Nthreads;
            cout << "Enter number of threads" << endl;
            cin >> Nthreads;

            calcForward<T, N, fix, M, Nz, Nr, detector>(inA, inT, inG, inN, inD, inNG, inDG, moveable, Nthreads, 0.0);
        }
    }

    return 0;
}
