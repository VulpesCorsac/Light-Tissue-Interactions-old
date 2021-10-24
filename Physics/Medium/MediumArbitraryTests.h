#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "MediumArbitrary.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;

class MediumArbitraryTests : public ::testing::Test {
protected:
    std::unique_ptr<MediumArbitrary<float>> medium = std::make_unique<MediumArbitrary<float>>();

    static constexpr float n0  = 2;
    static constexpr float nT  = 3;
    static constexpr float nD  = 4;
    static constexpr float nDT = 5;
    static constexpr float a0  = 6;
    static constexpr float aT  = 7;
    static constexpr float aD  = 8;
    static constexpr float aDT = 9;
    static constexpr float u0  = 10;
    static constexpr float uT  = 11;
    static constexpr float uD  = 12;
    static constexpr float uDT = 13;
    static constexpr float g0  = 14;
    static constexpr float gT  = 15;
    static constexpr float gD  = 16;
    static constexpr float gDT = 17;
    static constexpr float r0  = 18;
    static constexpr float rT  = 19;
    static constexpr float rD  = 20;
    static constexpr float rDT = 21;
    static constexpr float c0  = 22;
    static constexpr float cT  = 23;
    static constexpr float cD  = 24;
    static constexpr float cDT = 25;
    static constexpr float k0  = 26;
    static constexpr float kT  = 27;
    static constexpr float kD  = 28;
    static constexpr float kDT = 29;
    std::unique_ptr<MediumArbitrary<float>> nondefaultMedium = std::make_unique<MediumArbitrary<float>>(n0, nT, nD, nDT,
                                                                                                        a0, aT, aD, aDT,
                                                                                                        u0, uT, uD, uDT,
                                                                                                        g0, gT, gD, gDT,
                                                                                                        r0, rT, rD, rDT,
                                                                                                        c0, cT, cD, cDT,
                                                                                                        k0, kT, kD, kDT);
};

TEST_F(MediumArbitraryTests, TypeIsArbitrary) {
    EXPECT_EQ(medium->type, MediumType::Arbitrary);
}

TEST_F(MediumArbitraryTests, ConstructorFromParams) {
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (0, 0), n0);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (1, 0), n0 + nT);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (0, 1), n0 + nD);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (1, 1), n0 + nT + nD + nDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (0, 0), a0);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (1, 0), a0 + aT);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (0, 1), a0 + aD);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (1, 1), a0 + aT + aD + aDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (0, 0), u0);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (1, 0), u0 + uT);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (0, 1), u0 + uD);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (1, 1), u0 + uT + uD + uDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (0, 0), g0);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (1, 0), g0 + gT);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (0, 1), g0 + gD);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (1, 1), g0 + gT + gD + gDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (0, 0), r0);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (1, 0), r0 + rT);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (0, 1), r0 + rD);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (1, 1), r0 + rT + rD + rDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (0, 0), c0);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (1, 0), c0 + cT);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (0, 1), c0 + cD);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (1, 1), c0 + cT + cD + cDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(0, 0), k0);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(1, 0), k0 + kT);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(0, 1), k0 + kD);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(1, 1), k0 + kT + kD + kDT);
}

TEST_F(MediumArbitraryTests, ConstructorFromMediumProperties) {
    MediumProperties<float> properties(Physics_NS::MediumType::Arbitrary,
                                       n0, nT, nD, nDT,
                                       a0, aT, aD, aDT,
                                       u0, uT, uD, uDT,
                                       g0, gT, gD, gDT,
                                       r0, rT, rD, rDT,
                                       c0, cT, cD, cDT,
                                       k0, kT, kD, kDT);
    auto generatedMedium = std::make_unique<MediumArbitrary<float>>(properties);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (0, 0), n0);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (1, 0), n0 + nT);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (0, 1), n0 + nD);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (1, 1), n0 + nT + nD + nDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (0, 0), a0);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (1, 0), a0 + aT);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (0, 1), a0 + aD);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (1, 1), a0 + aT + aD + aDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (0, 0), u0);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (1, 0), u0 + uT);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (0, 1), u0 + uD);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (1, 1), u0 + uT + uD + uDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (0, 0), g0);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (1, 0), g0 + gT);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (0, 1), g0 + gD);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (1, 1), g0 + gT + gD + gDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (0, 0), r0);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (1, 0), r0 + rT);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (0, 1), r0 + rD);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (1, 1), r0 + rT + rD + rDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (0, 0), c0);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (1, 0), c0 + cT);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (0, 1), c0 + cD);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (1, 1), c0 + cT + cD + cDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(0, 0), k0);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(1, 0), k0 + kT);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(0, 1), k0 + kD);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(1, 1), k0 + kT + kD + kDT);
}

TEST_F(MediumArbitraryTests, GetDefaultRefraction) {
    EXPECT_FLOAT_EQ(medium->refraction(0, 0), 1);
    EXPECT_FLOAT_EQ(medium->refraction(0, 1), 1);
    EXPECT_FLOAT_EQ(medium->refraction(1, 0), 1);
    EXPECT_FLOAT_EQ(medium->refraction(1, 1), 1);
}

TEST_F(MediumArbitraryTests, GetDefaultAbsorption) {
    EXPECT_FLOAT_EQ(medium->absorption(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->absorption(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->absorption(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->absorption(1, 1), 0);
}

TEST_F(MediumArbitraryTests, GetDefaultScattering) {
    EXPECT_FLOAT_EQ(medium->scattering(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->scattering(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->scattering(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->scattering(1, 1), 0);
}

TEST_F(MediumArbitraryTests, GetDefaultScatteringAnisotropy) {
    EXPECT_FLOAT_EQ(medium->anisotropy(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->anisotropy(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->anisotropy(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->anisotropy(1, 1), 0);
}

TEST_F(MediumArbitraryTests, GetDefaultDensity) {
    EXPECT_FLOAT_EQ(medium->density(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->density(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->density(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->density(1, 1), 0);
}

TEST_F(MediumArbitraryTests, GetDefaultHeatCapacity) {
    EXPECT_FLOAT_EQ(medium->heat_capacity(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->heat_capacity(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->heat_capacity(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->heat_capacity(1, 1), 0);
}

TEST_F(MediumArbitraryTests, GetDefaultThermalConductivity) {
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(1, 1), 0);
}

TEST_F(MediumArbitraryTests, GetInteraction) {
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(0, 0), nondefaultMedium->absorption(0, 0) + nondefaultMedium->scattering(0, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(0, 1), nondefaultMedium->absorption(0, 1) + nondefaultMedium->scattering(0, 1));
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(1, 0), nondefaultMedium->absorption(1, 0) + nondefaultMedium->scattering(1, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(1, 1), nondefaultMedium->absorption(1, 1) + nondefaultMedium->scattering(1, 1));
}

TEST_F(MediumArbitraryTests, GetAlbedo) {
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(0, 0), nondefaultMedium->scattering(0, 0) / nondefaultMedium->interaction(0, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(0, 1), nondefaultMedium->scattering(0, 1) / nondefaultMedium->interaction(0, 1));
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(1, 0), nondefaultMedium->scattering(1, 0) / nondefaultMedium->interaction(1, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(1, 1), nondefaultMedium->scattering(1, 1) / nondefaultMedium->interaction(1, 1));
}

TEST_F(MediumArbitraryTests, GetThermalDiffusivity) {
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(0, 0), nondefaultMedium->thermal_conductivity(0, 0) / nondefaultMedium->density(0, 0) / nondefaultMedium->heat_capacity(0, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(0, 1), nondefaultMedium->thermal_conductivity(0, 1) / nondefaultMedium->density(0, 1) / nondefaultMedium->heat_capacity(0, 1));
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(1, 0), nondefaultMedium->thermal_conductivity(1, 0) / nondefaultMedium->density(1, 0) / nondefaultMedium->heat_capacity(1, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(1, 1), nondefaultMedium->thermal_conductivity(1, 1) / nondefaultMedium->density(1, 1) / nondefaultMedium->heat_capacity(1, 1));
}
