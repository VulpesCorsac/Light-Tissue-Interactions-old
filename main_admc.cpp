// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Scripts/DirectMC.h"
#include "Scripts/InverseMC.h"
#include "Scripts/SpoiltData.h"
/*
#include "Settings/Settings/SettingsImporterHelpers.h"
//*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace AddingDoubling_NS;

int main() {
    using namespace Inverse_NS;
    using namespace std;
    using T = double;

    constexpr int M = 32; // matrix size in Adding-Doubling

    constexpr int Nz = 1000;
    constexpr int Nr = 10000;
    constexpr bool detector = 1; // spheres => detector = 1; fiber => detector = 0.

    constexpr double TOLERANCE = 1e-4;

    constexpr int N = 2; /// minimize 2 parameters
    constexpr auto fix = FixedParameter::Tau;

    constexpr T nSlab = 1.4; /// refraction index of sample
    constexpr T nSlideTop = 1.5; /// refraction index of slide
    constexpr T nSlideBottom = 1.5;

    constexpr T rsmeas = 0.08531 - 0.0410959;
    constexpr T tcmeas = 0.338341;
    constexpr T tsmeas = 0.77350;

    const vector<pair<T,T>> Rd = {make_pair(0.0, rsmeas)};
    const vector<pair<T,T>> Td = {make_pair(0.0, tsmeas)};

    IntegratingSphere<T> SphereT(0.0508, 0.0125, 0.0); // dPort2 = zero if the sphere has one port
    IntegratingSphere<T> SphereR(0.0508, 0.0125, 0.0125);
    DetectorDistance<T> distances;
    distances.max  = 0.0;
    distances.min  = 0.0;
    distances.step = 0.0;

    const auto emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
    const auto glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
    const auto glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
    vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
    Sample<T> emptySample = (layers);

    T aOut, tauOut, gOut;
    T aStart, gStart, tStart;

    T fixedParamVal = fixParam<T,fix>(0.0, emptySample, tcmeas);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, 1000, 1,
                                              emptySample.getTotalThickness(), 10E-2, SphereR, SphereT,
                                              distances, Rd, Td, tcmeas);
    startingPoints(toMinimize, aStart, tStart, gStart, ModellingMethod::AD);
    toMinimize.InverseProblem(aStart, tStart, gStart, aOut, tauOut, gOut, ModellingMethod::AD);
    cerr << "a " << aOut << ", tau " << tauOut << ", g " << gOut << endl;

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
//        calcInverse<T,N,fix,M,Nz,Nr,detector>(settingsFname0, Nthreads);
    } else {
        int mode;
        cout << "SELECT MODE 0 1 2 3" << endl;
        cout << "MODE 0: Direct + inverse problem for fixed/moveable spheres and one set of parameters" << endl;
        cout << "MODE 1: Direct + inverse problem for fixed/moveable spheres and several sets of parameters (grid of taus 0.5 0.75 1.0 1.5 2.0 4.0)" << endl;
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

        T inA, inT, inG, inN, inD, inNglassTop, inDglassTop, inNglassBottom, inDglassBottom, inNG, inDG;
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
            spoiltData<T,2,FixedParameter::Tau,M,Nz,Nr,detector>(inA, inT, inG, inN, inD, inNG, inDG, inNG, inDG, moveable, Nthreads, err);
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
                spoiltData<T,2,FixedParameter::Tau,M,Nz,Nr,detector>(inA, inT, inG, inN, inD, inNG, inDG, inNG, inDG, moveable, Nthreads, err);
            }
        } else if (mode == 2) {
            cout << "Note that the first entries in diffuse reflection and transmission files will be used to estimate the initial point of MCML algorithm" << endl;
            int Nthreads;
            cout << "Enter number of threads" << endl;
            cin >> Nthreads;
            string settingsFname = "Settings & input files/SETTINGS.txt";

            inverseResults<T,Nz,Nr,detector> myRes = inverseMCfromFile<T,2,FixedParameter::Tau,M,Nz,Nr,detector>(settingsFname, Nthreads, ModellingMethod::AD, 1);
        } else if (mode == 3) {
            cout << "Enter tissue and glass parameters: a tau g nGlassTop dGlassTop n d nGlassBottom dGlassBottom" << endl;
            cin >> inA >> inT >> inG >> inNglassTop >> inDglassTop >> inN >> inD >> inNglassBottom >> inDglassBottom;

            bool moveable;
            cout << "Moveable = 1, fixed = 0" << endl;
            cin >> moveable;

            int Nthreads;
            cout << "Enter number of threads" << endl;
            cin >> Nthreads;

            MCresults<T,Nz,Nr,detector> myRes = directMC<T,Nz,Nr,detector>(inA, inT, inG, inNglassTop, inDglassTop, inN, inD, inNglassBottom, inDglassBottom, moveable, Nthreads, 1);
        } else if (mode == 4) {
            int Nthreads;
            cout << "Enter number of threads" << endl;
            cin >> Nthreads;
            string settingsFname = "Settings & input files/SETTINGS.txt";
            inverseResults<T,Nz,Nr,detector> myRes = inverseMCfromFile<T,2,FixedParameter::G,M,Nz,Nr,detector>(settingsFname, Nthreads, ModellingMethod::MC, 1);

        } else if (mode == 5) {
            int Nthreads;
            cout << "Enter number of threads" << endl;
            cin >> Nthreads;
            string settingsFname = "Settings & input files/SETTINGS.txt";
            inverseResults<T,Nz,Nr,detector> myRes = inverseMCfromFile<T,3,FixedParameter::G,M,Nz,Nr,detector>(settingsFname, Nthreads, ModellingMethod::AD, 1);
        }
    }

    return 0;
}
