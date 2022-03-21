// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Physics/Angles.h"
#include "Physics/BugerLambert.h"
#include "Physics/Reflectance.h"

#include "Utils/ReadFiles.h"

#include "MC/Detector.h"
#include "MC/InverseMonteCarlo.h"
#include "MC/Medium.h"
#include "MC/MonteCarlo.h"
#include "MC/MonteCarloMultithread.h"
#include "MC/Photon.h"
#include "MC/Sample.h"

#include "Inverse/FixedParam.h"

#include "AD/AddingDoublingNelderMead.h"
#include "AD/Quadrature.h"
#include "AD/RT.h"

// #include "Tests/TestIADStandalone.h"

/*
#include "Settings/Settings/SettingsImporterHelpers.h"
//*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <random>

using namespace AddingDoubling_NS;

template < typename T, size_t N, Inverse_NS::FixedParameter fix, size_t M, size_t Nz, size_t Nr, bool detector >
void calcAll(T inA, T inT, T inG, T inN, T inD, T inNG, T inDG, bool moveable, int Nthreads, double err) {
    using namespace Inverse_NS;
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

    DetectorDistance<T> distances;
    distances.max  = moveable ? 0.04 : 0.00;
    distances.min  = 0;
    distances.step = 0.002; // please, enter correct step for your borders

    constexpr int Nphotons = 1E6;
    constexpr T selectedRadius = 10E-2;

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(mySample, Nphotons, Nthreads, mySample.getTotalThickness(), selectedRadius, myResultsMT, SphereR, SphereT, distances);
    cout << myResultsMT << endl;

    /// TODO: Utils\Random.h
    static default_random_engine generator(time(nullptr));
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

    T nSlab = tissue.n;
    T nSlideTop, nSlideBottom;
    if (slides.empty()) {
        nSlideTop = tissue.n;
        nSlideBottom = tissue.n;
    } else {
        nSlideTop = slides[0].n;
        nSlideBottom = slides[1].n;
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
            rSpoilt.push_back({x.first, x.second + e1});
            diff += e1;
        }

        for (const auto& x: tsmeas) {
            T e2 = distribution(generator);
            tSpoilt.push_back({x.first, x.second + e2});
            diff += e2;
        }

        T tcSpoilt = tcmeas + distribution(generator);

        cout << diff << endl;

        for (const auto& x: rSpoilt)
            cout << x.first << " " << x.second << endl;

        for (const auto& x: tSpoilt)
            cout << x.first << " " << x.second << endl;

        T rStart = rSpoilt[0].second + myResultsMT.specularReflection; // the closest values to total Rs and Ts to be used in IAD algorithm
        T tStart = tSpoilt[0].second + tcSpoilt;

        cout << "rStart = " << rStart << " tStart = " << tStart << " tc = " << tcSpoilt << endl;

        /*
        T rStart = myResultsMT.diffuseReflection + myResultsMT.specularReflection; // the closest values to total Rs and Ts to be used in IAD algorithm
        T tStart = myResultsMT.diffuseTransmission + myResultsMT.BugerTransmission;
        //*/
        T g_val = 0.0;
        if (fix == FixedParameter::G && N == 2) {
            cerr << "Enter g " << endl;
            cin >> g_val;
        }
        T fixedParamVal = fixParam<T,M,N,fix>(g_val, nSlab, nSlideTop, nSlideBottom, tcSpoilt);
        T astart, gstart, tstart;
        Func<T,M,N,fix> toMinimize(fixedParamVal, nSlab, nSlideTop, nSlideBottom, rStart, tStart, tcSpoilt);
        startingPoints(toMinimize, astart, tstart, gstart);

        IAD<T,M,N,fix>(toMinimize, rStart, tStart, tcSpoilt, nSlab, nSlideTop, nSlideBottom, fixedParamVal, astart, tstart, gstart, aOutIAD, tauOutIAD, gOutIAD);

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

template < typename T, size_t N, Inverse_NS::FixedParameter fix, size_t M, size_t Nz, size_t Nr, bool detector >
void calcForward(T inA, T inT, T inG, T inN, T inD, T inNG, T inDG, bool moveable, int Nthreads, double err) {
    using namespace Inverse_NS;
    using namespace std;

    auto tissue = Medium<T>::fromAlbedo(inN, inA, inT, inD, inG);
    auto emptyTissue = Medium<T>::fromAlbedo(inN, 0.0, 0.0, inD, 0.0);
    auto glass = Medium<T>::fromCoeffs(inNG, 0.0, 0.0, inDG, 0.0);
    vector<Medium<T>> layers = {glass, tissue, glass};
    Sample<T> mySample(layers, 1.0, 1.0);
    // vector<Medium<T>> slides = {glass, glass};
    vector<Medium<T>> emptyLayers = {glass, emptyTissue, glass};
    Sample<T> emptySample (emptyLayers, 1.0, 1.0);

    IntegratingSphere<T> SphereT(0.1, 0.0125, 0.0); // dPort2 = zero if the sphere has one port
    IntegratingSphere<T> SphereR(0.1, 0.0125, 0.0125);

    DetectorDistance<T> distances;
    distances.max  = moveable ? 0.04 : 0.00;
    distances.min  = 0;
    distances.step = 0.002; // please, enter correct step for your borders

    constexpr int Nphotons = 1E6;
    constexpr T selectedRadius = 10E-2;

    MCresults<T,Nz,Nr,detector> myResultsMT;
    MCmultithread<T,Nz,Nr,detector>(mySample, Nphotons, Nthreads, mySample.getTotalThickness(), selectedRadius, myResultsMT, SphereR, SphereT, distances);
    cout << myResultsMT << endl;

    static default_random_engine generator(time(nullptr));
    normal_distribution<T> distribution(0, err);

    auto rsmeas = myResultsMT.detectedR;
    auto tsmeas = myResultsMT.detectedT;
    T tcmeas = myResultsMT.BugerTransmission;
    auto ameas = myResultsMT.matrixA;
    auto anglesR = myResultsMT.arrayAnglesR;
    auto anglesT = myResultsMT.arrayAnglesT;

    ofstream Rdfile;
    ofstream Tdfile;
    ofstream Tcfile;
    ofstream Afile;
    ofstream Anglesfile;

    Rdfile.open("Output files/Rd_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");
    Tdfile.open("Output files/Td_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");
    Tcfile.open("Output files/Tc_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");
    Afile.open("Output files/A_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".csv");
    Anglesfile.open("Output files/Angles_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".csv");

    /// TODO: throw exception if can't open. Anyway you try to do something with the file.
    if (!Rdfile.is_open())
        cout << "Failed to open file Rd" << endl;
    if (!Tdfile.is_open())
        cout << "Failed to open file Td" << endl;
    if (!Tcfile.is_open())
        cout << "Failed to open file Tc" << endl;
    if (!Afile.is_open())
        cout << "Failed to open file A" << endl;
    if (!Anglesfile.is_open())
        cout << "Failed to open file A" << endl;

    const static IOFormat CSVFormat(FullPrecision, DontAlignCols, ", ", "\n");
    for (const auto& x: rsmeas)
        Rdfile << x.first << "\t" << x.second << '\n';
    for (const auto& x: tsmeas)
        Tdfile << x.first << "\t" << x.second << '\n';
    Tcfile << 0.000 << "\t" << tcmeas << '\n';
    Afile << ameas.format(CSVFormat) << '\n';
    Anglesfile << anglesR.format(CSVFormat) << ", " << anglesT.format(CSVFormat) << '\n';

    /*
    cout << "R" << '\n';
    cout << anglesR << '\n';
    cout << "T" << '\n';
    cout << anglesT << '\n';
    //*/
    Rdfile.close();
    Tdfile.close();
    Tcfile.close();
    Afile.close();
    Anglesfile.close();
}

template < typename T, size_t N, Inverse_NS::FixedParameter fix, size_t M, size_t Nz, size_t Nr, bool detector >
void calcInverse(const std::string& settingsFile, int Nthreads) {
    using namespace Inverse_NS;
    using namespace Physics_NS;
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

    T nSlab = tissue.n;
    T nSlideTop, nSlideBottom;
    if (slides.empty()) {
        nSlideTop = tissue.n;
        nSlideBottom = tissue.n;
    } else {
        nSlideTop = slides[0].n;
        nSlideBottom = slides[1].n;
    }
    //*/

    int Nphotons;
    constexpr T selectedRadius = 10E-2;

    Sample<T> emptySample;
    IntegratingSphere<T> SphereR, SphereT;
    bool moveable;
    vector<pair<T,T>> Rd, Td, Tc;
    readSettings<T, fix>(settingsFile, emptySample, SphereR, SphereT, moveable, Nphotons, Rd, Td, Tc);

    DetectorDistance<T> distances;
    distances.max  = T(Rd[Rd.size()-1].first);
    distances.min  = T(Rd[0].first);
    distances.step = T(Rd[Rd.size()-1].first - Rd[Rd.size() - 2].first); // please, enter correct step for your borders

    Medium<T> emptyTissue = Medium<T>::fromAlbedo(0.0, 0.0, 0.0, 0.0, 0.0);
    T rSpec, nSlab, nSlideTop, nSlideBottom;
    vector<Medium<T>> slides = {};

    if (emptySample.getNlayers() == 1) {
        rSpec = FresnelReflectance(emptySample.getNvacUpper(), emptySample.getMedium(0).n, 1.0);
        emptyTissue = emptySample.getMedium(0);
        nSlab = emptyTissue.n;
        nSlideTop = emptyTissue.n;
        nSlideBottom = emptyTissue.n;
    } else {
        slides = {emptySample.getMedium(0), emptySample.getMedium(emptySample.getNlayers() - 1)};
        T r1 = FresnelReflectance(emptySample.getNvacUpper(), emptySample.getMedium(0).n, 1.0);
        T r2 = FresnelReflectance(emptySample.getMedium(0).n, emptySample.getMedium(1).n, 1.0);
        rSpec = (r1 + r2 - 2 * r1 * r2) / (1 - r1 * r2);
        emptyTissue = emptySample.getMedium(1); // yeah i need to make several layers possible
        nSlab = emptyTissue.n;
        nSlideTop = slides[0].n;
        nSlideBottom = slides[1].n;
    }

    T aOutIAD, tauOutIAD, gOutIAD; // IAD result will be recorded here
    T aOutIMC, tauOutIMC, gOutIMC; // IMC result will be recorded here
    T checkConvEps = 1E-3;


    /*
    cout << "Enter starting points R T" << endl;
    T inRstart, inTstart;
    cin >> inRstart >> inTstart;
    T rStart = inRstart + rSpec; // the closest values to total Rs and Ts to be used in IAD algorithm
    T tStart = inTstart;
    //*/

    if ((fix == FixedParameter::G && N == 2) || N == 3)
        Tc.push_back({static_cast<T>(0.0), static_cast<T>(0.0)});
    T rStart = Rd[0].second + rSpec; // the closest values to total Rs and Ts to be used in IAD algorithm
    T tStart = Td[0].second;
    if (SphereT.getDPort2() != 0)
        tStart += Tc[0].second;

    T g_val = 0.0;
	if (fix == FixedParameter::G && N == 2) {
        cerr << "Enter g " << endl;
        cin >> g_val;
    }
    T fixedParamVal = fixParam<T,M,N,fix>(g_val, nSlab, nSlideTop, nSlideBottom, Tc[0].second);
    T astart, gstart, tstart;
    Func<T,M,N,fix> toMinimize(fixedParamVal, nSlab, nSlideTop, nSlideBottom, rStart, tStart, Tc[0].second);
    startingPoints(toMinimize, astart, tstart, gstart);
    IAD<T,M,N,fix>(toMinimize, rStart, tStart, Tc[0].second, nSlab, nSlideTop, nSlideBottom, fixedParamVal, astart, tstart, gstart, aOutIAD, tauOutIAD, gOutIAD);
    cout << "First approximation: Inverse Adding-Doubling" << endl;
    cout << "a = " << aOutIAD << ", tau = " << tauOutIAD << ", g = " << gOutIAD << endl;
    IMC<T,Nz,Nr,detector,N,fix>(Rd, Td, Tc[0].second, emptyTissue, move(slides), Nphotons, Nthreads, emptySample.getTotalThickness(), selectedRadius, SphereR, SphereT, distances, aOutIAD, tauOutIAD, gOutIAD, checkConvEps, aOutIMC, tauOutIMC, gOutIMC);


    cout << "Absorption coefficient ua = " << tauOutIMC * (1 - aOutIMC) / emptyTissue.D << endl;
    cout << "Scattering coefficient us = " << tauOutIMC * aOutIMC / emptyTissue.D << endl;
    cout << "Scattering anisotropy g = " << gOutIMC << endl;


    auto tissueFin = Medium<T>::fromAlbedo(nSlab, aOutIMC, tauOutIMC, emptyTissue.D, gOutIMC);
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

    Rdfile.open("Output files/Rd_" + to_string(aOutIMC) + "_" + to_string(tauOutIMC) + "_" + to_string(gOutIMC) + ".txt");
    Tdfile.open("Output files/Td_" + to_string(aOutIMC) + "_" + to_string(tauOutIMC) + "_" + to_string(gOutIMC) + ".txt");
    Tcfile.open("Output files/Tc_" + to_string(aOutIMC) + "_" + to_string(tauOutIMC) + "_" + to_string(gOutIMC) + ".txt");

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

int main() {
    using namespace Inverse_NS;

    using namespace std;
    using T = double;

    constexpr int N = 2; // minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr int M = 32; // matrix size in Adding-Doubling

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1; // spheres => detector = 1; fiber => detector = 0.

    /// CALCULATE ENTRIES FOR MINIMIZATION WITH MONTE-CARLO, OR USE INPUT FILE
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
        calcInverse<T,N,fix,M,Nz,Nr,detector>(settingsFname0, Nthreads);
    } else {
        int mode;
        cout << "SELECT MODE 0 1 2 3" << endl;
        cout << "MODE 0: Forward + inverse problem for fixed/moveable spheres and one set of parameters" << endl;
        cout << "MODE 1: Forward + inverse problem for fixed/moveable spheres and several sets of parameters (grid of taus 0.5 0.75 1.0 1.5 2.0 4.0)" << endl;
        cout << "MODE 2: find optical properties from settings file" << endl;
        cout << "MODE 3: R(z) and T(z) for fixed/moveable spheres and one set of parameters" << endl;
        cout << "MODE 4: find optical properties from settings file, FIX G AND CLOSED UPPER SPHERE" << endl;
        cout << "MODE 5: find optical properties from settings file, 3 params AND CLOSED UPPER SPHERE" << endl;
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
        } else if (mode == 4) {
            int Nthreads;
            cout << "Enter number of threads" << endl;
            cin >> Nthreads;
            string settingsFname1 = "Settings & input files/SETTINGS.txt";
            calcInverse<T, N, FixedParameter::G, M, Nz, Nr, detector>(settingsFname1, Nthreads);

        } else if (mode == 5) {
            int Nthreads;
            cout << "Enter number of threads" << endl;
            cin >> Nthreads;
            string settingsFname1 = "Settings & input files/SETTINGS.txt";
            calcInverse<T, 3, fix, M, Nz, Nr, detector>(settingsFname1, Nthreads);
        }
    }

    return 0;
}
