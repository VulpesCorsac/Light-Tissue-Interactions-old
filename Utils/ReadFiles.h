#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "../MC/Medium.h"
#include "../MC/MonteCarlo.h"
#include "../MC/Sample.h"
#include "../MC/Detector.h"

using namespace std;


template < typename T >
void readTable (vector<pair<T,T>>& table, const string& fileName ) {
    ifstream myFileStream(fileName);
    if (!myFileStream.is_open()){
        cout << "Failed to open file " << fileName << endl;
    }
    T z, val;
    string line, zString, valString;
    while(getline(myFileStream, line)) {
        stringstream ss(line);
        getline(ss, zString, '\t');
        getline(ss, valString, '\n');
        table.push_back(make_pair(T(stod(zString)), T(stod(valString))));
    }
 /*   for (auto x : table)
        cout << x.first << " " << x.second << endl;*/
    myFileStream.close();
}

template < typename T >
void readSettings (const string& fileName, Sample<T>& emptySample, IntegratingSphere<T>& SphereR, IntegratingSphere<T>& SphereT,
                   bool& moveable, int& Np, vector<pair<T,T>>& Rd, vector<pair<T,T>>& Td, vector<pair<T,T>>& Tc) {
    ifstream myFileStream(fileName);
    if (!myFileStream.is_open()){
        cout << "Failed to open settings file " << fileName << endl;
    }
    string line, nLayersLine, nLine, dLine, cache3, DLine, d1Line, d2Line, cache, mLine, NpLine;
    string RdFname, TdFname, TcFname;
    int nLayers, Nphotons;
    vector<Medium<T>> emptyLayers;


    for (int lineno = 0; getline (myFileStream,line) && lineno < 40; ++lineno) {
        stringstream ss(line);
        if (lineno == 7) {
            getline(ss, nLayersLine, '\n');
            nLayers = stoi(nLayersLine);
        }
        if (lineno > 9 && lineno < 9 + nLayers + 1) {
            getline(ss, dLine, '\t');
            getline(ss, nLine, '\n');
            emptyLayers.push_back(Medium<T>::fromAlbedo(T(stod(nLine)), 0.0, 0.0, T(stod(dLine)), 0.0));
            emptySample = Sample<T>(emptyLayers, 1.0, 1.0);
        }
        if (lineno == 13 + nLayers) {
            getline(ss, DLine, '\t');
            getline(ss, d1Line, '\t');
            getline(ss, d2Line, '\n');
            SphereR = IntegratingSphere<T>(T(stod(DLine))*1.0, T(stod(d1Line))*1.0, T(stod(d2Line))*1.0);
        }
        if (lineno == 15 + nLayers) {
            getline(ss, DLine, '\t');
            getline(ss, d1Line, '\t');
            getline(ss, d2Line, '\n');
            SphereT = IntegratingSphere<T>(T(stod(DLine))*1.0, T(stod(d1Line))*1.0, T(stod(d2Line))*1.0);
        }
        if (lineno == 18 + nLayers) {
            getline(ss, mLine, '\n');
            if (mLine == "1")
                moveable = true;
            else
                moveable = false;
        }

        if (lineno == 21 + nLayers) {
            getline(ss, NpLine, '\n');
            Nphotons = stoi(NpLine);
        }
        if (lineno == 25 + nLayers)
            getline(ss, RdFname, '\n');
        if (lineno == 27 + nLayers)
            getline(ss, TdFname, '\n');
        if (lineno == 29 + nLayers)
            getline(ss, TcFname, '\n');
    }
    readTable(Rd, "Settings & input files/" + RdFname);
    readTable(Td, "Settings & input files/" + TdFname);
    readTable(Tc, "Settings & input files/" + TcFname);
}
