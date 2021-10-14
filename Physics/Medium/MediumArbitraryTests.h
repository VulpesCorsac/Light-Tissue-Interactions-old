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
    std::unique_ptr<MediumArbitrary<float>> nondefaultMedium = std::make_unique<MediumArbitrary<float>>(n0, nT, nD, nDT,
                                                                                                        a0, aT, aD, aDT,
                                                                                                        u0, uT, uD, uDT,
                                                                                                        g0, gT, gD, gDT);
};

TEST_F(MediumArbitraryTests, TypeIsArbitrary) {
    EXPECT_EQ(medium->type, MediumType::Arbitrary);
}

TEST_F(MediumArbitraryTests, ConstructorFromParams) {
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction(0, 0), n0);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction(1, 0), n0 + nT);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction(0, 1), n0 + nD);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction(1, 1), n0 + nT + nD + nDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption(0, 0), a0);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption(1, 0), a0 + aT);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption(0, 1), a0 + aD);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption(1, 1), a0 + aT + aD + aDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering(0, 0), u0);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering(1, 0), u0 + uT);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering(0, 1), u0 + uD);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering(1, 1), u0 + uT + uD + uDT);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy(0, 0), g0);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy(1, 0), g0 + gT);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy(0, 1), g0 + gD);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy(1, 1), g0 + gT + gD + gDT);
}

TEST_F(MediumArbitraryTests, ConstructorFromMediumProperties) {
    MediumProperties<float> properties;
    properties.n0  = n0;
    properties.nT  = nT;
    properties.nD  = nD;
    properties.nDT = nDT;
    properties.a0  = a0;
    properties.aT  = aT;
    properties.aD  = aD;
    properties.aDT = aDT;
    properties.u0  = u0;
    properties.uT  = uT;
    properties.uD  = uD;
    properties.uDT = uDT;
    properties.g0  = g0;
    properties.gT  = gT;
    properties.gD  = gD;
    properties.gDT = gDT;
    auto generatedMedium = std::make_unique<MediumArbitrary<float>>(properties);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(0, 0), n0);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(1, 0), n0 + nT);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(0, 1), n0 + nD);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(1, 1), n0 + nT + nD + nDT);
    EXPECT_FLOAT_EQ(generatedMedium->absorption(0, 0), a0);
    EXPECT_FLOAT_EQ(generatedMedium->absorption(1, 0), a0 + aT);
    EXPECT_FLOAT_EQ(generatedMedium->absorption(0, 1), a0 + aD);
    EXPECT_FLOAT_EQ(generatedMedium->absorption(1, 1), a0 + aT + aD + aDT);
    EXPECT_FLOAT_EQ(generatedMedium->scattering(0, 0), u0);
    EXPECT_FLOAT_EQ(generatedMedium->scattering(1, 0), u0 + uT);
    EXPECT_FLOAT_EQ(generatedMedium->scattering(0, 1), u0 + uD);
    EXPECT_FLOAT_EQ(generatedMedium->scattering(1, 1), u0 + uT + uD + uDT);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy(0, 0), g0);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy(1, 0), g0 + gT);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy(0, 1), g0 + gD);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy(1, 1), g0 + gT + gD + gDT);
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
