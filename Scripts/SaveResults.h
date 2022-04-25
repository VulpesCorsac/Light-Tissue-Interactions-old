#pragma once

#include "../MC/MonteCarlo.h"

#include <fstream>
#include <string>
#include <sstream>

template < typename T, size_t Nz, size_t Nr, bool detector>
void saveResults(const MCresults<T,Nz,Nr,detector>& results, const T& inA, const T& inT, const T& inG, bool matrixExport) {
    using namespace std;
    auto rsmeas = results.detectedR;
    auto tsmeas = results.detectedT;
    T tcmeas = results.BugerTransmission;

    ofstream Rdfile, Tdfile, Tcfile;

    Rdfile.open("Output files/Rd_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");
    Tdfile.open("Output files/Td_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");
    Tcfile.open("Output files/Tc_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".txt");

    if (!Rdfile.is_open())
        throw invalid_argument("Failed to open file Rd");
    if (!Tdfile.is_open())
        throw invalid_argument("Failed to open file Td");
    if (!Tcfile.is_open())
        throw invalid_argument("Failed to open file Tc");

    for (const auto& x: rsmeas)
        Rdfile << x.first << "\t" << x.second << '\n';
    for (const auto& x: tsmeas)
        Tdfile << x.first << "\t" << x.second << '\n';
    Tcfile << 0.000 << "\t" << tcmeas << '\n';

    Rdfile.close();
    Tdfile.close();
    Tcfile.close();

    if (matrixExport) {
        auto ameas = results.heatSource;
        auto anglesR = results.arrayAnglesR;
        auto anglesT = results.arrayAnglesT;
        auto source = results.sourceMatrix;

        ofstream Afile, Anglesfile, Sourcefile;

        Afile.open("Output files/A_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".csv");
        Anglesfile.open("Output files/Angles_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".csv");
        Sourcefile.open("Output files/Source_" + to_string(inA) + "_" + to_string(inT) + "_" + to_string(inG) + ".csv");
        if (!Afile.is_open())
            throw invalid_argument("Failed to open file A");
        if (!Anglesfile.is_open())
            throw invalid_argument("Failed to open file Angles");
        if (!Sourcefile.is_open())
            throw invalid_argument("Failed to open file Sources");

        const static IOFormat CSVFormat(FullPrecision, DontAlignCols, ", ", "\n");
        Afile << ameas.format(CSVFormat) << '\n';
        Anglesfile << anglesR.format(CSVFormat) << ", " << anglesT.format(CSVFormat) << '\n';

        for (const auto& x: source)
            Sourcefile << x.first << ", " << x.second << "\n";

        Afile.close();
        Anglesfile.close();
        Sourcefile.close();
    }
}
