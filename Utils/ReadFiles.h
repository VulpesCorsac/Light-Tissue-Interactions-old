#pragma once

#include "../MC/Detector.h"
#include "../MC/Medium.h"
#include "../MC/MonteCarlo.h"
#include "../MC/Sample.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "../Inverse/FixedParam.h"
#include "../Utils/Contracts.h"

#include <utility>

template < typename T >
void readTable(std::vector<std::pair<T,T>>& table, const std::string& fileName) {
    using namespace std;

    ifstream myFileStream(fileName);
    if (!myFileStream.is_open())
        cout << "Failed to open file " << fileName << endl;

    // T z, val;
    string line, zString, valString;
    while(getline(myFileStream, line)) {
        stringstream ss(line);
        getline(ss, zString, '\t');
        getline(ss, valString, '\n');
        table.push_back({static_cast<T>(stod(zString)),
                         static_cast<T>(stod(valString))});
    }
     /*
     for (auto x : table)
        cout << x.first << " " << x.second << endl;
    //*/
    myFileStream.close();
}

template < typename T >
void readTableKinetics(std::vector<std::pair<T,T>>& table, const T& dist, const std::string& fileName) {
    using namespace std;

    ifstream myFileStream(fileName);
    if (!myFileStream.is_open())
        cout << "Failed to open file " << fileName << endl;

    // T z, val;
    string line, zString, valString;
    while(getline(myFileStream, line)) {
        stringstream ss(line);
        getline(ss, zString, '\t');
        getline(ss, valString, '\n');
        table.push_back({dist, static_cast<T>(stod(valString))});
    }
    myFileStream.close();
}

template < typename T >
void getTimesKinetics(std::vector<T>& times, const std::string& fileName) {
    using namespace std;

    ifstream myFileStream(fileName);
    if (!myFileStream.is_open())
        cout << "Failed to open file " << fileName << endl;

    // T z, val;
    string line, zString, valString;
    while(getline(myFileStream, line)) {
        stringstream ss(line);
        getline(ss, zString, '\t');
        getline(ss, valString, '\n');
        times.push_back(static_cast<T>(stod(zString)));
    }
    myFileStream.close();
}

template < typename T, Inverse_NS::FixedParameter fix >
void readSettings(const std::string& fileName,
                  Sample<T>& emptySample,
                  IntegratingSphere<T>& SphereR,
                  IntegratingSphere<T>& SphereT,
                  int& Nphotons,
                  std::vector<std::pair<T,T>>& Rd,
                  std::vector<std::pair<T,T>>& Td,
                  std::vector<std::pair<T,T>>& Tc,
                  LightSource<T>& source,
                  bool kinetics,
                  std::vector<T>& kineticTimes) {
    using namespace std;
    using namespace Inverse_NS;

    bool moveable;
    ifstream myFileStream(fileName);
    if (!myFileStream.is_open())
        cout << "Failed to open settings file " << fileName << endl;
    string line, nLayersLine, nLine, dLine, cache3, DLine, d1Line, d2Line, cache, mLine, NpLine;
    string sourceTypeLine, sourceRadLine;
    SourceType sourceType;
    string RdFname, TdFname, TcFname;
    int nLayers;
    vector<Medium<T>> emptyLayers;


    for (int lineno = 0; getline(myFileStream,line) && lineno < 40; ++lineno) {
        stringstream ss(line);
        if (lineno == 7) {
            getline(ss, nLayersLine, '\n');
            nLayers = stoi(nLayersLine);
        }
        if (lineno > 9 && lineno < 9 + nLayers + 1) {
            getline(ss, dLine, '\t');
            getline(ss, nLine, '\n');
            emptyLayers.push_back(Medium<T>::fromAlbedo(static_cast<T>(stod(nLine)),
                                                        0.0,
                                                        0.0,
                                                        static_cast<T>(stod(dLine)), 0.0));
            emptySample = Sample<T>(emptyLayers, 1.0, 1.0);
        }
        if (lineno == 13 + nLayers) {
            getline(ss, DLine, '\t');
            getline(ss, d1Line, '\t');
            getline(ss, d2Line, '\n');
            SphereR = IntegratingSphere<T>(static_cast<T>(stod(DLine)) *1.0,
                                           static_cast<T>(stod(d1Line))*1.0,
                                           static_cast<T>(stod(d2Line))*1.0);
        }
        if (lineno == 15 + nLayers) {
            getline(ss, DLine, '\t');
            getline(ss, d1Line, '\t');
            getline(ss, d2Line, '\n');
            SphereT = IntegratingSphere<T>(static_cast<T>(stod(DLine)) *1.0,
                                           static_cast<T>(stod(d1Line))*1.0,
                                           static_cast<T>(stod(d2Line))*1.0);
        }
        if (lineno == 19 + nLayers) {
            getline(ss, sourceTypeLine, '\t');
            getline(ss, sourceRadLine, '\n');
            if (sourceTypeLine == "Point")
                sourceType = SourceType::Point;
            else if (sourceTypeLine == "Gaussian")
                sourceType = SourceType::Gaussian;
            else if (sourceTypeLine == "Circle")
                sourceType = SourceType::Circle;
            source = LightSource(static_cast<T>(stod(sourceRadLine)), sourceType);
        }
        if (lineno == 22 + nLayers) {
            getline(ss, mLine, '\n');
            if (mLine == "1")
                moveable = true;
            else
                moveable = false;
        }

        if (lineno == 25 + nLayers) {
            getline(ss, NpLine, '\n');
            Nphotons = stoi(NpLine);
        }
        if (lineno == 29 + nLayers)
            getline(ss, RdFname, '\n');
        if (lineno == 31 + nLayers)
            getline(ss, TdFname, '\n');
        if (lineno == 33 + nLayers)
            getline(ss, TcFname, '\n');
    }

    if (kinetics) {
        T dist;
        cerr << "Enter distances between sample and sphere port (equal for both spheres)" << endl;
        cin >> dist;
        readTableKinetics(Rd, dist, "Settings & input files/" + RdFname);
        readTableKinetics(Td, dist, "Settings & input files/" + TdFname);
        readTableKinetics(Tc, 0.0,   "Settings & input files/" + TcFname);
        getTimesKinetics(kineticTimes, "Settings & input files/" + RdFname);
    } else {
        readTable(Rd, "Settings & input files/" + RdFname);
        readTable(Td, "Settings & input files/" + TdFname);
        readTable(Tc, "Settings & input files/" + TcFname);
    }
}
