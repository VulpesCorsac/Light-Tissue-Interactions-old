#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "../Inverse/InverseProblem.h"
#include "../Inverse/StartingPoints.h"
#include "../MC/Medium.h"
#include "../MC/Sample.h"
#include "../Scripts/InverseMC.h"

#include <gtest/gtest.h>

using namespace std;
using namespace Inverse_NS;

template < typename T, size_t N, Inverse_NS::FixedParameter fix, size_t M, size_t Nz, size_t Nr, bool detector >
class TestIMC {
public:
    TestIMC(const T& nSlabNew, const T& nSlideTopNew, const T& nSlideBottomNew,
            const string& fNameNew, const T& gNew, const int& NthNew, const int& NphNew) EXCEPT_INPUT_PARAMS
            : nSlab(nSlabNew)
            , nSlideTop(nSlideTopNew)
            , nSlideBottom(nSlideBottomNew)
            , fName(fNameNew)
            , gVal(gNew)
            , Nphotons(NphNew)
            , Nthreads(NthNew)
    {

        setValues();
        inverseResults<T,Nz,Nr,detector> inverseResultsFin = inverseMC<T,N,fix,M,Nz,Nr,detector>(RdMeas, TdMeas, TcMeas, emptySample,
                                                                                                 SphereR, SphereT, source, distances,
                                                                                                 Nphotons, selectedRadius, Nthreads, gVal, ModellingMethod::MC, 0);
        aOut = inverseResultsFin.a;
        tauOut = inverseResultsFin.tau;
        gOut = inverseResultsFin.g;
    }

    void setValues() {
        readTable(RdMeas, "Tests/IMC Test Data/Rd_" + fName + ".txt");
        readTable(TdMeas, "Tests/IMC Test Data/Td_" + fName + ".txt");
        readTable(TcMeas, "Tests/IMC Test Data/Tc_" + fName + ".txt");

        distances.max  = T(RdMeas[RdMeas.size()-1].first);
        distances.min  = T(RdMeas[0].first);
        if (RdMeas.size() == 1)
            distances.step = 0.0;
        else
            distances.step = T(RdMeas[RdMeas.size()-1].first - RdMeas[RdMeas.size() - 2].first);

        SphereT = IntegratingSphere<T>(0.0508, 0.0125, 0.0);
        SphereR = IntegratingSphere<T>(0.0508, 0.0125, 0.0125);
        source = LightSource<T>(0.0, SourceType::Point);

        emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, 1.0, 0.0);
        glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, 1.0, 0.0);
        glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, 1.0, 0.0);
        vector<Medium<T>> layers = {glassTop, emptyTissue, glassBottom};
        emptySample = Sample<T>(layers);
    }

    T getAout() const noexcept { return aOut; }
    T getTout() const noexcept { return tauOut; }
    T getGout() const noexcept { return gOut; }
protected:
    T nSlab, nSlideTop, nSlideBottom;
    T gVal;
    vector<pair<T,T>> RdMeas, TdMeas, TcMeas;
    string fName;
    IntegratingSphere<T> SphereR, SphereT;
    DetectorDistance<T> distances;
    LightSource<T> source;
    Medium<T> emptyTissue, glassTop, glassBottom;
    Sample<T> emptySample;
    int Nphotons, Nthreads;
    const T selectedRadius = 1E-1;
    T aOut, tauOut, gOut;
};
TEST(InverseMonteCarlo, AbsorptionOnly1) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.000000_1.000000_0.000000";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.5, 1.5, 1.5, fname, 0.0, 4, 10000);
    EXPECT_NEAR(test.getAout(), 0.9, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1  , TOLERANCE);
}