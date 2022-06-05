// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Scripts/DirectMC.h"
#include "Scripts/DirectHeterogeneousMC.h"
#include "Scripts/InverseMC.h"
#include "Scripts/SpoiltData.h"
#include "Scripts/KineticsIMC.h"
//#include "MC/MonteCarloTests.h"
//#include "MC/InverseMonteCarloTests.h"
//#include "AD/InverseAddingDoublingTests.h"
//#include "AD/AddingDoublingTests.h"
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

    constexpr int M = 8; // matrix size in Adding-Doubling

    constexpr int Nz = 98;
    constexpr int Nr = 501;
    constexpr bool detector = 1; // spheres => detector = 1; fiber => detector = 0.
/*
    auto coagMatrix = coag<T, Nz, Nr>();
    ofstream Afile;
    Afile.open("Output files/CoagMatrix.csv");
    if (!Afile.is_open())
        throw invalid_argument("Failed to open file A");
    const static IOFormat CSVFormat(FullPrecision, DontAlignCols, ", ", "\n");
    Afile << coagMatrix.format(CSVFormat) << '\n';
    Afile.close();

    MCresults<T,Nz,Nr,detector> myRes = directHeterogeneousMC<T,Nz,Nr,detector>(0.83, 0.6, 0.0, 1.5, 0.0, 1.5, 0.001, 1.5, 0.0, coagMatrix, 0, 1, 1);
*/

    int mode;
    cout << "SELECT MODE:" << endl;
    cout << "MODE 0: R(z) and T(z) for fixed/moveable spheres and one set of parameters" << endl;
    cout << "MODE 1: find optical properties from settings file, fixed Tau" << endl;
    cout << "MODE 2: find optical properties from settings file, fixed G" << endl;
    cout << "MODE 3: find optical properties from settings file, 3 parameters minimization" << endl;
    cout << "MODE 4: find optical properties for kinetics data, fixed Tau" << endl;
    cout << "THE FOLLOWING MODES ARE FOR INVERSE PROBLEM TESTING (ADDING ERRORS TO DATA):" << endl;
    cout << "MODE 5: Direct + inverse problem for fixed/moveable spheres and one set of parameters" << endl;
    cout << "MODE 6: Direct + inverse problem for fixed/moveable spheres and several sets of parameters (grid of taus 0.5 0.75 1.0 1.5 2.0 4.0)" << endl;
    cin >> mode;

    string settingsFname = "Settings & input files/SETTINGS.txt";

    T inA, inT, inG, inN, inD, inNglassTop, inDglassTop, inNglassBottom, inDglassBottom, inNG, inDG;
    if (mode == 0) {
        cout << "Enter tissue and glass parameters: a tau g nGlassTop dGlassTop n d nGlassBottom dGlassBottom" << endl;
        cin >> inA >> inT >> inG >> inNglassTop >> inDglassTop >> inN >> inD >> inNglassBottom >> inDglassBottom;

        bool moveable;
        cout << "Moveable = 1, fixed = 0" << endl;
        cin >> moveable;

        int Nthreads;
        cout << "Enter number of threads" << endl;
        cin >> Nthreads;
        MCresults<T,Nz,Nr,detector> myRes = directMC<T,Nz,Nr,detector>(inA, inT, inG, inNglassTop, inDglassTop, inN, inD, inNglassBottom, inDglassBottom, moveable, Nthreads, 1);
    } else if (mode == 1) {
        int Nthreads;
        cout << "Enter number of threads" << endl;
        cin >> Nthreads;
        inverseResults<T,Nz,Nr,detector> myRes = inverseMCfromFile<T,2,FixedParameter::Tau,M,Nz,Nr,detector>(settingsFname, Nthreads, ModellingMethod::MC, 1);
    } else if (mode == 2) {
        int Nthreads;
        cout << "Enter number of threads" << endl;
        cin >> Nthreads;
        inverseResults<T,Nz,Nr,detector> myRes = inverseMCfromFile<T,2,FixedParameter::G,M,Nz,Nr,detector>(settingsFname, Nthreads, ModellingMethod::MC, 1);
    } else if (mode == 3) {
        int Nthreads;
        cout << "Enter number of threads" << endl;
        cin >> Nthreads;
        inverseResults<T,Nz,Nr,detector> myRes = inverseMCfromFile<T,3,FixedParameter::G,M,Nz,Nr,detector>(settingsFname, Nthreads, ModellingMethod::AD, 1);
    } else if (mode == 4) {
        int Nthreads;
        cout << "Enter number of threads" << endl;
        cin >> Nthreads;
        kineticsIMC<T,2,FixedParameter::Tau,M,Nz,Nr,detector>(settingsFname, Nthreads, ModellingMethod::MC, 1);
    } else if (mode == 5) {
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
    } else if (mode == 6) {
        Matrix<T, 1, 6> gridT;
        Matrix<T, 1, 7> gridA, gridG;
        gridT << 0.5, 0.75, 1.0, 1.5, 2.0, 4.0;
        gridA << 0.0001, 0.25, 0.5, 0.75, 0.9, 0.95, 0.999;
        gridG << 0.0001, 0.25, 0.5, 0.75, 0.9, 0.95, 0.999;
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
    }

    return 0;
}
