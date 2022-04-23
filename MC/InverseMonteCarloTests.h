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
    TestIMC(const T& nSlabNew, const T& dSlabNew, const T& nSlideTopNew, const T& dSlideTopNew, const T& nSlideBottomNew, const T& dSlideBottomNew,
            const string& fNameNew, const T& gNew, const int& NthNew, const int& NphNew, ModellingMethod startMod) EXCEPT_INPUT_PARAMS
            : nSlab(nSlabNew)
            , dSlab(dSlabNew)
            , nSlideTop(nSlideTopNew)
            , dSlideTop(dSlideTopNew)
            , nSlideBottom(nSlideBottomNew)
            , dSlideBottom(dSlideBottomNew)
            , fName(fNameNew)
            , gVal(gNew)
            , Nphotons(NphNew)
            , Nthreads(NthNew)
    {

        setValues();
        inverseResults<T,Nz,Nr,detector> inverseResultsFin = inverseMC<T,N,fix,M,Nz,Nr,detector>(RdMeas, TdMeas, TcMeas, emptySample,
                                                                                                 SphereR, SphereT, source, distances,
                                                                                                 Nphotons, selectedRadius, Nthreads, gVal, startMod, 0);
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

        vector<Medium<T>> layers;
        emptyTissue = Medium<T>::fromAlbedo(nSlab, 0.0, 0.0, dSlab, 0.0);
        if (dSlideTop != 0 && dSlideBottom != 0) {
            auto glassTop = Medium<T>::fromAlbedo(nSlideTop, 0.0, 0.0, dSlideTop, 0.0);
            auto glassBottom = Medium<T>::fromAlbedo(nSlideBottom, 0.0, 0.0, dSlideBottom, 0.0);
            layers = {glassTop, emptyTissue, glassBottom};
        } else if (dSlideTop == 0 && dSlideBottom == 0)
            layers = {emptyTissue};
        else
            throw invalid_argument("It seems that you want to calculate 2 layers, you can only do 1 or 3");
        emptySample = Sample<T>(layers);
    }

    T getAout() const noexcept { return aOut; }
    T getTout() const noexcept { return tauOut; }
    T getGout() const noexcept { return gOut; }
protected:
    T nSlab, dSlab, nSlideTop, dSlideTop, nSlideBottom, dSlideBottom;
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

TEST(InverseMonteCarlo, AbsorptionOnly_MFP_IS_D) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.000000_1.000000_0.000000";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.5, 0.001, 1.5, 0.0, 1.5, 0.0, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.0, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1  , TOLERANCE);
}

TEST(InverseMonteCarlo, AbsorptionOnly_MFP_GT_D) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.000000_0.100000_0.000000";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.3, 0.001, 1.3, 0.0, 1.3, 0.0, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.0, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 0.1, TOLERANCE);
}

TEST(InverseMonteCarlo, AbsorptionOnly_MFP_LT_D) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.000000_10.000000_0.000000";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.6, 0.01, 1.6, 0.0, 1.6, 0.0, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.0, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10 , TOLERANCE);
}

TEST(InverseMonteCarlo, AbsorptionOnly_MFP_IS_D_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.000000_1.000000_0.000000_glass";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.5, 0.001, 1.6, 0.001, 1.6, 0.001, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.0, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1  , TOLERANCE);
}

TEST(InverseMonteCarlo, AbsorptionOnly_MFP_GT_D_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.000000_0.100000_0.000000_glass";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.3, 0.001, 1.4, 0.001, 1.4, 0.001, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.0, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 0.1, TOLERANCE);
}

TEST(InverseMonteCarlo, AbsorptionOnly_MFP_LT_D_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.000000_10.000000_0.000000_glass";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.6, 0.01, 1.65, 0.001, 1.65, 0.001, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.0, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10 , TOLERANCE);
}

TEST(InverseMonteCarlo, Gmin_AbsorptionScattering_MFP_IS_D) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.900000_1.000000_0.900000";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.5, 0.001, 1.5, 0.0, 1.5, 0.0, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.9, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9, TOLERANCE);
}

TEST(InverseMonteCarlo, Gmin_AbsorptionScattering_MFP_GT_D) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.800000_0.100000_0.500000";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.3, 0.001, 1.3, 0.0, 1.3, 0.0, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.8, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 0.1, TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.5, TOLERANCE);
}

TEST(InverseMonteCarlo, Gmin_AbsorptionScattering_MFP_LT_D) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.300000_10.000000_0.000000";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.6, 0.01, 1.6, 0.0, 1.6, 0.0, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.3, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10 , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.0, TOLERANCE);
}

TEST(InverseMonteCarlo, Gmin_AbsorptionScattering_TISSUE) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.990000_10.000000_0.900000";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.4, 0.0002, 1.4, 0.0, 1.4, 0.0, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.99, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9 , TOLERANCE);
}

TEST(InverseMonteCarlo, Gmin_AbsorptionScattering_MFP_IS_D_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.900000_1.000000_0.900000_glass";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.5, 0.001, 1.6, 0.001, 1.6, 0.001, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.9, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9, TOLERANCE);
}

TEST(InverseMonteCarlo, Gmin_AbsorptionScattering_MFP_GT_D_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.800000_0.100000_0.500000_glass";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.3, 0.001, 1.4, 0.001, 1.4, 0.001, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.8, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 0.1, TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.5, TOLERANCE);
}

TEST(InverseMonteCarlo, Gmin_AbsorptionScattering_MFP_LT_D_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.300000_10.000000_0.000000_glass";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.6, 0.01, 1.65, 0.001, 1.65, 0.001, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.3, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10 , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.0, TOLERANCE);
}

TEST(InverseMonteCarlo, Gmin_AbsorptionScattering_TISSUE_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.990000_10.000000_0.900000_glass";
    TestIMC<double, 2, FixedParameter::Tau, 4, 1000, 10000, 1> test(1.4, 0.0002, 1.5, 0.001, 1.5, 0.001, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.99, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9 , TOLERANCE);
}

///

TEST(InverseMonteCarlo, Tmin_AbsorptionScattering_MFP_IS_D) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.900000_1.000000_0.900000";
    TestIMC<double, 2, FixedParameter::G, 4, 1000, 10000, 1> test(1.5, 0.001, 1.5, 0.0, 1.5, 0.0, fname, 0.9, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.9, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9, TOLERANCE);
}

TEST(InverseMonteCarlo, Tmin_AbsorptionScattering_MFP_GT_D) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.800000_0.100000_0.500000";
    TestIMC<double, 2, FixedParameter::G, 4, 1000, 10000, 1> test(1.3, 0.001, 1.3, 0.0, 1.3, 0.0, fname, 0.5, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.8, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 0.1, TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.5, TOLERANCE);
}

TEST(InverseMonteCarlo, Tmin_AbsorptionScattering_MFP_LT_D) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.300000_10.000000_0.000000";
    TestIMC<double, 2, FixedParameter::G, 4, 1000, 10000, 1> test(1.6, 0.01, 1.6, 0.0, 1.6, 0.0, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.3, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10 , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.0, TOLERANCE);
}

TEST(InverseMonteCarlo, Tmin_AbsorptionScattering_TISSUE) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.990000_10.000000_0.900000";
    TestIMC<double, 2, FixedParameter::G, 4, 1000, 10000, 1> test(1.4, 0.0002, 1.4, 0.0, 1.4, 0.0, fname, 0.9, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.99, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9 , TOLERANCE);
}

TEST(InverseMonteCarlo, Tmin_AbsorptionScattering_MFP_IS_D_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.900000_1.000000_0.900000_glass";
    TestIMC<double, 2, FixedParameter::G, 4, 1000, 10000, 1> test(1.5, 0.001, 1.6, 0.001, 1.6, 0.001, fname, 0.9, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.9, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 1  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9, TOLERANCE);
}

TEST(InverseMonteCarlo, Tmin_AbsorptionScattering_MFP_GT_D_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.800000_0.100000_0.500000_glass";
    TestIMC<double, 2, FixedParameter::G, 4, 1000, 10000, 1> test(1.3, 0.001, 1.4, 0.001, 1.4, 0.001, fname, 0.5, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.8, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 0.1, TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.5, TOLERANCE);
}

TEST(InverseMonteCarlo, Tmin_AbsorptionScattering_MFP_LT_D_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.300000_10.000000_0.000000_glass";
    TestIMC<double, 2, FixedParameter::G, 4, 1000, 10000, 1> test(1.6, 0.01, 1.65, 0.001, 1.65, 0.001, fname, 0.0, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.3, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10 , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.0, TOLERANCE);
}

TEST(InverseMonteCarlo, Tmin_AbsorptionScattering_TISSUE_Glass) {
    constexpr double TOLERANCE = 1e-2;
    string fname = "0.990000_10.000000_0.900000_glass";
    TestIMC<double, 2, FixedParameter::G, 4, 1000, 10000, 1> test(1.4, 0.0002, 1.5, 0.001, 1.5, 0.001, fname, 0.9, 4, 10000, ModellingMethod::MC);
    EXPECT_NEAR(test.getAout(), 0.99, TOLERANCE);
    EXPECT_NEAR(test.getTout(), 10  , TOLERANCE);
    EXPECT_NEAR(test.getGout(), 0.9 , TOLERANCE);
}
