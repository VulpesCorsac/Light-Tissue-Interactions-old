#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "MediumLinear.h"

#include <gtest/gtest.h>

#include <memory>

using namespace Physics_NS;

class MediumLinearTests : public ::testing::Test {
protected:
    std::unique_ptr<MediumLinear<float>> medium = std::make_unique<MediumLinear<float>>();

    static constexpr float n0 = 2;
    static constexpr float nT = 3;
    static constexpr float nD = 4;
    static constexpr float a0 = 5;
    static constexpr float aT = 6;
    static constexpr float aD = 7;
    static constexpr float u0 = 8;
    static constexpr float uT = 9;
    static constexpr float uD = 10;
    static constexpr float g0 = 11;
    static constexpr float gT = 12;
    static constexpr float gD = 13;
    static constexpr float r0 = 14;
    static constexpr float rT = 15;
    static constexpr float rD = 16;
    static constexpr float c0 = 17;
    static constexpr float cT = 18;
    static constexpr float cD = 19;
    static constexpr float k0 = 20;
    static constexpr float kT = 21;
    static constexpr float kD = 22;
    std::unique_ptr<MediumLinear<float>> nondefaultMedium = std::make_unique<MediumLinear<float>>(n0, nT, nD,
                                                                                                  a0, aT, aD,
                                                                                                  u0, uT, uD,
                                                                                                  g0, gT, gD,
                                                                                                  r0, rT, rD,
                                                                                                  c0, cT, cD,
                                                                                                  k0, kT, kD);
};

TEST_F(MediumLinearTests, TypeIsLinear) {
    EXPECT_EQ(medium->type, MediumType::Linear);
}

TEST_F(MediumLinearTests, ConstructorFromParams) {
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (0, 0), n0);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (1, 0), n0 + nT);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (0, 1), n0 + nD);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction          (1, 1), n0 + nT + nD);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (0, 0), a0);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (1, 0), a0 + aT);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (0, 1), a0 + aD);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption          (1, 1), a0 + aT + aD);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (0, 0), u0);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (1, 0), u0 + uT);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (0, 1), u0 + uD);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering          (1, 1), u0 + uT + uD);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (0, 0), g0);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (1, 0), g0 + gT);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (0, 1), g0 + gD);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy          (1, 1), g0 + gT + gD);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (0, 0), r0);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (1, 0), r0 + rT);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (0, 1), r0 + rD);
    EXPECT_FLOAT_EQ(nondefaultMedium->density             (1, 1), r0 + rT + rD);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (0, 0), c0);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (1, 0), c0 + cT);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (0, 1), c0 + cD);
    EXPECT_FLOAT_EQ(nondefaultMedium->heat_capacity       (1, 1), c0 + cT + cD);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(0, 0), k0);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(1, 0), k0 + kT);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(0, 1), k0 + kD);
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_conductivity(1, 1), k0 + kT + kD);
}

TEST_F(MediumLinearTests, ConstructorFromMediumProperties) {
    MediumProperties<float> properties(Physics_NS::MediumType::Linear,
                                       n0, nT, nD, std::nullopt,
                                       a0, aT, aD, std::nullopt,
                                       u0, uT, uD, std::nullopt,
                                       g0, gT, gD, std::nullopt,
                                       r0, rT, rD, std::nullopt,
                                       c0, cT, cD, std::nullopt,
                                       k0, kT, kD, std::nullopt);
    auto generatedMedium = std::make_unique<MediumLinear<float>>(properties);
    EXPECT_FLOAT_EQ(generatedMedium->refraction          (0, 0), n0);
    EXPECT_FLOAT_EQ(generatedMedium->refraction          (1, 0), n0 + nT);
    EXPECT_FLOAT_EQ(generatedMedium->refraction          (0, 1), n0 + nD);
    EXPECT_FLOAT_EQ(generatedMedium->refraction          (1, 1), n0 + nT + nD);
    EXPECT_FLOAT_EQ(generatedMedium->absorption          (0, 0), a0);
    EXPECT_FLOAT_EQ(generatedMedium->absorption          (1, 0), a0 + aT);
    EXPECT_FLOAT_EQ(generatedMedium->absorption          (0, 1), a0 + aD);
    EXPECT_FLOAT_EQ(generatedMedium->absorption          (1, 1), a0 + aT + aD);
    EXPECT_FLOAT_EQ(generatedMedium->scattering          (0, 0), u0);
    EXPECT_FLOAT_EQ(generatedMedium->scattering          (1, 0), u0 + uT);
    EXPECT_FLOAT_EQ(generatedMedium->scattering          (0, 1), u0 + uD);
    EXPECT_FLOAT_EQ(generatedMedium->scattering          (1, 1), u0 + uT + uD);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy          (0, 0), g0);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy          (1, 0), g0 + gT);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy          (0, 1), g0 + gD);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy          (1, 1), g0 + gT + gD);
    EXPECT_FLOAT_EQ(generatedMedium->density             (0, 0), r0);
    EXPECT_FLOAT_EQ(generatedMedium->density             (1, 0), r0 + rT);
    EXPECT_FLOAT_EQ(generatedMedium->density             (0, 1), r0 + rD);
    EXPECT_FLOAT_EQ(generatedMedium->density             (1, 1), r0 + rT + rD);
    EXPECT_FLOAT_EQ(generatedMedium->heat_capacity       (0, 0), c0);
    EXPECT_FLOAT_EQ(generatedMedium->heat_capacity       (1, 0), c0 + cT);
    EXPECT_FLOAT_EQ(generatedMedium->heat_capacity       (0, 1), c0 + cD);
    EXPECT_FLOAT_EQ(generatedMedium->heat_capacity       (1, 1), c0 + cT + cD);
    EXPECT_FLOAT_EQ(generatedMedium->thermal_conductivity(0, 0), k0);
    EXPECT_FLOAT_EQ(generatedMedium->thermal_conductivity(1, 0), k0 + kT);
    EXPECT_FLOAT_EQ(generatedMedium->thermal_conductivity(0, 1), k0 + kD);
    EXPECT_FLOAT_EQ(generatedMedium->thermal_conductivity(1, 1), k0 + kT + kD);
}

TEST_F(MediumLinearTests, GetDefaultRefraction) {
    EXPECT_FLOAT_EQ(medium->refraction(0, 0), 1);
    EXPECT_FLOAT_EQ(medium->refraction(0, 1), 1);
    EXPECT_FLOAT_EQ(medium->refraction(1, 0), 1);
    EXPECT_FLOAT_EQ(medium->refraction(1, 1), 1);
}

TEST_F(MediumLinearTests, GetDefaultAbsorption) {
    EXPECT_FLOAT_EQ(medium->absorption(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->absorption(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->absorption(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->absorption(1, 1), 0);
}

TEST_F(MediumLinearTests, GetDefaultScattering) {
    EXPECT_FLOAT_EQ(medium->scattering(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->scattering(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->scattering(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->scattering(1, 1), 0);
}

TEST_F(MediumLinearTests, GetDefaultScatteringAnisotropy) {
    EXPECT_FLOAT_EQ(medium->anisotropy(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->anisotropy(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->anisotropy(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->anisotropy(1, 1), 0);
}

TEST_F(MediumLinearTests, GetDefaultDensity) {
    EXPECT_FLOAT_EQ(medium->density(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->density(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->density(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->density(1, 1), 0);
}

TEST_F(MediumLinearTests, GetDefaultHeatCapacity) {
    EXPECT_FLOAT_EQ(medium->heat_capacity(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->heat_capacity(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->heat_capacity(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->heat_capacity(1, 1), 0);
}

TEST_F(MediumLinearTests, GetDefaultThermalConductivity) {
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(0, 0), 0);
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(0, 1), 0);
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(1, 0), 0);
    EXPECT_FLOAT_EQ(medium->thermal_conductivity(1, 1), 0);
}

TEST_F(MediumLinearTests, GetInteraction) {
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(0, 0), nondefaultMedium->absorption(0, 0) + nondefaultMedium->scattering(0, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(0, 1), nondefaultMedium->absorption(0, 1) + nondefaultMedium->scattering(0, 1));
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(1, 0), nondefaultMedium->absorption(1, 0) + nondefaultMedium->scattering(1, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->interaction(1, 1), nondefaultMedium->absorption(1, 1) + nondefaultMedium->scattering(1, 1));
}

TEST_F(MediumLinearTests, GetAlbedo) {
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(0, 0), nondefaultMedium->scattering(0, 0) / nondefaultMedium->interaction(0, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(0, 1), nondefaultMedium->scattering(0, 1) / nondefaultMedium->interaction(0, 1));
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(1, 0), nondefaultMedium->scattering(1, 0) / nondefaultMedium->interaction(1, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->albedo(1, 1), nondefaultMedium->scattering(1, 1) / nondefaultMedium->interaction(1, 1));
}

TEST_F(MediumLinearTests, GetThermalDiffusivity) {
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(0, 0), nondefaultMedium->thermal_conductivity(0, 0) / nondefaultMedium->density(0, 0) / nondefaultMedium->heat_capacity(0, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(0, 1), nondefaultMedium->thermal_conductivity(0, 1) / nondefaultMedium->density(0, 1) / nondefaultMedium->heat_capacity(0, 1));
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(1, 0), nondefaultMedium->thermal_conductivity(1, 0) / nondefaultMedium->density(1, 0) / nondefaultMedium->heat_capacity(1, 0));
    EXPECT_FLOAT_EQ(nondefaultMedium->thermal_diffusivity(1, 1), nondefaultMedium->thermal_conductivity(1, 1) / nondefaultMedium->density(1, 1) / nondefaultMedium->heat_capacity(1, 1));
}
