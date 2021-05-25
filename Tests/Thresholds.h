#pragma once

#include "../MC/Medium.h"
#include "../MC/BugerLambert.h"
#include "../MC/MonteCarlo.h"
#include "../MC/MCmultithread.h"

#include <iostream>

using namespace std;

using T = double;

static constexpr size_t Nz = 1e3;
static constexpr size_t Nr = 1e4;

class MC_General {
public:
    static constexpr T selectedRadius = 1e-1;

    static constexpr int single_thread = 1;
    static constexpr int multi_thread  = 4;

public:
    template < typename T >
    struct TestResult {
        T SPECULAR_REFLECTION  = 0;
        T DIFFUSE_REFLECTION   = 0;
        T DIFFUSE_TRANSMISSION = 0;

        T SPECULAR_REFLECTION_TOLERANCE  = 0;
        T DIFFUSE_REFLECTION_TOLERANCE   = 0;
        T DIFFUSE_TRANSMISSION_TOLERANCE = 0;
    };

    static constexpr T TOTAL_TOLERANCE = 1e-4;
};

class MC_AbsorptionOnly : public MC_General {
public:
    const Medium<T> tissue1{1.5, 1000, 0, 1e-3, 0.9}; // MFP == D
    const Medium<T> tissue2{1.3,  100, 0, 1e-3, 0.5}; // MFP == D * 10
    const Medium<T> tissue3{1.6, 1000, 0, 1e-2, 0.0}; // MFP == D * 0.1
};

class MC_SingleLayerAbsorptionOnly_MFP_IS_D : public MC_AbsorptionOnly {
public:
    const Sample<T> sample{{tissue1}, 1, 1};
    static constexpr int photons = 1e6;
    const TestResult<T> EXPECTED{0.04, 0.004989, 0.3390, 1e-4, 1e-3, 2e-3};
    const T BUGER = BugerLambert<T>(1, 1.5, 1, 1);
};

class MC_SingleLayerAbsorptionOnly_MFP_GREATER_THAN_D : public MC_AbsorptionOnly {
public:
    const Sample<T> sample{{tissue2}, 1, 1};
    static constexpr int photons = 1e6;
    const TestResult<T> EXPECTED{0.0170132, 0.01346, 0.8743, 1e-4, 1e-3, 2e-3};
    const T BUGER = BugerLambert<T>(0.1, 1.3, 1, 1);
};

class MC_SingleLayerAbsorptionOnly_MFP_LESS_THAN_D : public MC_AbsorptionOnly {
public:
    const Sample<T> sample{{tissue3}, 1, 1};
    static constexpr int photons = 5e6;
    const TestResult<T> EXPECTED{0.05325, 0, 0.00004069, 1e-4, 1e-3, 9e-6};
    const T BUGER = BugerLambert<T>(10, 1.6, 1, 1);
};

class MC_MultiLayerAbsorptionOnly_MFP_IS_D : public MC_AbsorptionOnly {
public:
    const Medium<T> glass{1.6 , 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue1, glass}, 1, 1};
    static constexpr int photons = 1e6;
    const TestResult<T> EXPECTED{0.05419, 0.00656, 0.3292, 1e-4, 1e-3, 2e-3};
    const T BUGER = BugerLambert<T>(1, 1.5, 1.6, 1.6);
};

class MC_MultiLayerAbsorptionOnly_MFP_GREATER_THAN_D : public MC_AbsorptionOnly {
public:
    const Medium<T> glass{1.4 , 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue2, glass}, 1, 1};
    static constexpr int photons = 1e6;
    const TestResult<T> EXPECTED{0.02907, 0.02244, 0.853, 1e-4, 1e-3, 2e-3};
    const T BUGER = BugerLambert<T>(0.1, 1.3, 1.4, 1.4);
};

class MC_MultiLayerAbsorptionOnly_MFP_LESS_THAN_D : public MC_AbsorptionOnly {
public:
    const Medium<T> glass{1.65, 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue3, glass}, 1, 1};
    static constexpr int photons = 2e7;
    const TestResult<T> EXPECTED{0.06037, 0, 0.00004008, 1e-4, 1e-3, 8e-6};
    const T BUGER = BugerLambert<T>(10, 1.6, 1, 1);
};

class MC_AbsorptionScattering : public MC_General {
public:
    const Medium<T> tissue1{1.5, 100, 900,  1e-3, 0.9}; // MFP == D
    const Medium<T> tissue2{1.3,  20,  80,  1e-3, 0.5}; // MFP == D * 10
    const Medium<T> tissue3{1.6, 700, 300, 10e-3, 0.0}; // MFP == D * 0.1
};

class MC_SingleLayerAbsorptionScattering_MFP_IS_D : public MC_AbsorptionScattering {
public:
    const Sample<T> sample{{tissue1}, 1, 1};
    const TestResult<T> EXPECTED{0.04, 0.0435, 0.767, 1e-4, 1e-3, 2e-3};
    static constexpr int photons = 5e5;
};

class MC_SingleLayerAbsorptionScattering_MFP_GREATER_THAN_D : public MC_AbsorptionScattering {
public:
    const Sample<T> sample{{tissue2}, 1, 1};
    const TestResult<T> EXPECTED{0.01701, 0.0272, 0.9206, 1e-4, 1e-3, 1e-3};
    static constexpr int photons = 5e5;
};

class MC_SingleLayerAbsorptionScattering_MFP_LESS_THAN_D : public MC_AbsorptionScattering {
public:
    const Sample<T> sample{{tissue3}, 1, 1};
    const TestResult<T> EXPECTED{0.05325, 0.0175, 0.0000549, 2e-4, 1e-3, 3e-5};
    static constexpr int photons = 1e6;
};

class MC_MultiLayerAbsorptionScattering_MFP_IS_D : public MC_AbsorptionScattering {
public:
    const Medium<T> glass1{1.6 , 0, 0, 1e-3, 0};
    const Medium<T> glass3{1.65, 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass1, tissue1, glass3}, 1, 1};
    const TestResult<T> EXPECTED{0.05419, 0.05813, 0.7394, 1e-4, 5e-3, 2e-3};
    static constexpr int photons = 1e6;
};

class MC_MultiLayerAbsorptionScattering_MFP_GREATER_THAN_D : public MC_AbsorptionScattering {
public:
    const Medium<T> glass{1.4 , 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue2, glass}, 1, 1};
    const TestResult<T> EXPECTED{0.02907, 0.03695, 0.8987, 1e-4, 1e-3, 1e-3};
    static constexpr int photons = 1e6;
};

class MC_MultiLayerAbsorptionScattering_MFP_LESS_THAN_D : public MC_AbsorptionScattering {
public:
    const Medium<T> glass{1.65, 0, 0, 1e-3, 0};
    const Sample<T> sample{{glass, tissue3, glass}, 1, 1};
    const TestResult<T> EXPECTED{0.06037, 0.01718, 0.0000541, 1e-4, 1e-3, 2e-5};
    static constexpr int photons = 1e6;
};

struct Deviation {
    T specularReflectionDeviation  = 0;
    T diffuseReflectionDeviation   = 0;
    T diffuseTransmissionDeviation = 0;
};

std::ostream& operator << (std::ostream& os, const Deviation& d) noexcept {
    os << "SR: "  << d.specularReflectionDeviation;
    os << ", DR: " << d.diffuseReflectionDeviation;
    os << ", DT: "   << d.diffuseTransmissionDeviation;
    return os;
}

Deviation Max(const Deviation& d, const MCresults<T,Nz,Nr>& r, const MC_General::TestResult<T>& expected) {
    Deviation ans;

    ans.specularReflectionDeviation  = max(abs(r.specularReflection  - expected.SPECULAR_REFLECTION ), d.specularReflectionDeviation );
    ans.diffuseReflectionDeviation   = max(abs(r.diffuseReflection   - expected.DIFFUSE_REFLECTION  ), d.diffuseReflectionDeviation  );
    ans.diffuseTransmissionDeviation = max(abs(r.diffuseTransmission - expected.DIFFUSE_TRANSMISSION), d.diffuseTransmissionDeviation);
    return ans;
}

#define RUNS 100
#define FUNC(Fixture) {                                                            \
    Fixture fixture;                                                               \
    Deviation ans;                                                                 \
    for (int i = 0; i < RUNS; ++i) {                                               \
        if (i % 100 == 0)                                                          \
            cout << #Fixture << ", run " << i << ", " << ans << endl;              \
        ans = Max(ans,                                                             \
                  MonteCarlo<T, Nz, Nr>(fixture.sample,                            \
                                        fixture.photons,                           \
                                        fixture.sample.getTotalThickness(),        \
                                        fixture.selectedRadius).CalculateResult(), \
                  fixture.EXPECTED);                                               \
    }                                                                              \
    cout << #Fixture << " " << ans << endl;                                        \
}                                                                                  \

void evaluateThresholds() {
    FUNC(MC_SingleLayerAbsorptionOnly_MFP_IS_D)
    FUNC(MC_SingleLayerAbsorptionOnly_MFP_GREATER_THAN_D)
    FUNC(MC_SingleLayerAbsorptionOnly_MFP_LESS_THAN_D)
    FUNC(MC_MultiLayerAbsorptionOnly_MFP_IS_D)
    FUNC(MC_MultiLayerAbsorptionOnly_MFP_GREATER_THAN_D)
    FUNC(MC_MultiLayerAbsorptionOnly_MFP_LESS_THAN_D)
    FUNC(MC_SingleLayerAbsorptionScattering_MFP_IS_D)
    FUNC(MC_SingleLayerAbsorptionScattering_MFP_GREATER_THAN_D)
    FUNC(MC_SingleLayerAbsorptionScattering_MFP_LESS_THAN_D)
    FUNC(MC_MultiLayerAbsorptionScattering_MFP_IS_D)
    FUNC(MC_MultiLayerAbsorptionScattering_MFP_GREATER_THAN_D)
    FUNC(MC_MultiLayerAbsorptionScattering_MFP_LESS_THAN_D)
}
