#pragma once

#include "../Utils/ReadFiles.h"
#include "../Inverse/InverseProblem.h"
#include "../Inverse/StartingPoints.h"

template < typename T, size_t Nz, size_t Nr, bool detector >
struct inverseResults {
    T a;
    T tau;
    T g;
    MCresults<T,Nz,Nr,detector> resultsMC;
};

template < typename T, size_t N, Inverse_NS::FixedParameter fix, size_t M, size_t Nz, size_t Nr, bool detector >
inverseResults<T,Nz,Nr,detector> inverseMC(const std::vector<std::pair<T,T>>& Rd, const std::vector<std::pair<T,T>>& Td, const std::vector<std::pair<T,T>>& Tc,
                                           const Sample<T>& emptySample, const IntegratingSphere<T>& SphereR, const IntegratingSphere<T>& SphereT,
                                           const DetectorDistance<T>& distances, const int& Nphotons, const T& selectedRadius, const bool& moveable, const int& Nthreads,
                                           Inverse_NS::ModellingMethod startMod, const bool& save) {
    using namespace Inverse_NS;
    using namespace Physics_NS;
    using namespace std;

    T g_val = 0.0;
    if (fix == FixedParameter::G && N == 2) {
        cerr << "Enter g " << endl;
        cin >> g_val;
    }
    T fixedParamVal = fixParam<T,fix>(g_val, emptySample, Tc[0].second);
    Func<T,Nz,Nr,detector,M,N,fix> toMinimize(fixedParamVal, emptySample, Nphotons, Nthreads,
                                              emptySample.getTotalThickness(), selectedRadius, SphereR, SphereT,
                                              distances, Rd, Td, Tc[0].second);

    T aStart, tStart, gStart;
    startingPoints(toMinimize, aStart, tStart, gStart, startMod);
    cerr << "Nearest point in starting grid:" << endl;
    cerr << "a = " << aStart << ", tau = " << tStart << ", g = " << gStart << endl;
    if (startMod == ModellingMethod::AD){
        cerr << "Starting first approximation with IAD" << endl;
        T tMeas = Td[0].second;
        T rMeas = Rd[0].second;
        T tcMeas = Tc[0].second;
        T rSpec;
        if (emptySample.getNlayers() == 1)
            rSpec = FresnelReflectance<T>(emptySample.getNvacUpper(), emptySample.getNslab(), 1.0);
        else if (emptySample.getNlayers() == 3) {
            T r1 = FresnelReflectance<T>(emptySample.getNvacUpper(), emptySample.getNslideTop(), 1.0);
            T r2 = FresnelReflectance<T>(emptySample.getNslideTop(), emptySample.getNslab(), 1.0);
            rSpec = (r1 + r2 - 2 * r1 * r2) / (1 - r1 * r2);
        } else
            throw invalid_argument("Only one or three layers possible");
        rMeas += rSpec;
        if (SphereT.getDPort2() != 0)
            tMeas += tcMeas;
        T aStartIAD, tStartIAD, gStartIAD;
        Func<T,Nz,Nr,detector,M,N,fix> firstMinimize(fixedParamVal, emptySample, rMeas, tMeas, tcMeas);
        cerr << rMeas << " " << tMeas << endl;
        firstMinimize.InverseProblem(aStart, tStart, gStart, aStartIAD, tStartIAD, gStartIAD, ModellingMethod::AD);
        aStart = aStartIAD;
        tStart = tStartIAD;
        gStart = gStartIAD;
        cerr << "First approximation: Inverse Adding-Doubling" << endl;
        cerr << "a = " << aStart << ", tau = " << tStart << ", g = " << gStart << endl;
    }
    T aOutIMC, tOutIMC, gOutIMC;
    toMinimize.InverseProblem(aStart, tStart, gStart, aOutIMC, tOutIMC, gOutIMC, ModellingMethod::MC);


    cerr << "Absorption coefficient ua = " << tOutIMC * (1 - aOutIMC) / toMinimize.getEmptySample().getTurbidMedium().getD() << endl;
    cerr << "Scattering coefficient us = " << tOutIMC * aOutIMC / toMinimize.getEmptySample().getTurbidMedium().getD() << endl;
    cerr << "Scattering anisotropy g = " << gOutIMC << endl;


    auto tissueFin = Medium<T>::fromAlbedo(emptySample.getNslab(), aOutIMC, tOutIMC, toMinimize.getEmptySample().getTurbidMedium().getD(), gOutIMC);
    vector<Medium<T>> layersFin;
    if (emptySample.getNlayers() == 1)
        layersFin = {tissueFin};
    else if (emptySample.getNlayers() == 3)
        layersFin = {emptySample.getMedium(0), tissueFin, emptySample.getMedium(2)};
    else
        throw invalid_argument("Be a darling and feed this program either 1 or 3 layers fin");
    Sample<T> mySampleFin(layersFin);

    MCresults<T,Nz,Nr,detector> MCresultsFin;
    MCmultithread<T,Nz,Nr,detector>(mySampleFin, Nphotons, Nthreads, mySampleFin.getTotalThickness(), selectedRadius, MCresultsFin, SphereR, SphereT, distances);
    cout << MCresultsFin << endl;

    inverseResults<T,Nz,Nr,detector> inverseResultsFin;
    inverseResultsFin.a = aOutIMC;
    inverseResultsFin.tau = tOutIMC;
    inverseResultsFin.g = gOutIMC;
    inverseResultsFin.resultsMC = MCresultsFin;

    return inverseResultsFin;
}

template < typename T, size_t N, Inverse_NS::FixedParameter fix, size_t M, size_t Nz, size_t Nr, bool detector >
inverseResults<T,Nz,Nr,detector> inverseMCfromFile(const std::string& settingsFile, const int& Nthreads, Inverse_NS::ModellingMethod startMod, const bool& save) {
    using namespace Inverse_NS;
    using namespace Physics_NS;
    using namespace std;

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

    inverseResults<T,Nz,Nr,detector> inverseResultsFin = inverseMC<T,N,fix,M,Nz,Nr,detector>(Rd, Td, Tc, emptySample, SphereR, SphereT,
                                                                   distances, Nphotons, selectedRadius, moveable, Nthreads, startMod, save);

    return inverseResultsFin;
}


