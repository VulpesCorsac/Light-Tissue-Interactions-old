#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

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
    std::unique_ptr<MediumLinear<float>> nondefaultMedium = std::make_unique<MediumLinear<float>>(n0, nT, nD,
                                                                                                  a0, aT, aD,
                                                                                                  u0, uT, uD,
                                                                                                  g0, gT, gD);
};

TEST_F(MediumLinearTests, TypeIsLinear) {
    EXPECT_EQ(medium->type, MediumType::Linear);
}

TEST_F(MediumLinearTests, ConstructorFromParams) {
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction(0, 0), n0);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction(1, 0), n0 + nT);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction(0, 1), n0 + nD);
    EXPECT_FLOAT_EQ(nondefaultMedium->refraction(1, 1), n0 + nT + nD);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption(0, 0), a0);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption(1, 0), a0 + aT);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption(0, 1), a0 + aD);
    EXPECT_FLOAT_EQ(nondefaultMedium->absorption(1, 1), a0 + aT + aD);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering(0, 0), u0);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering(1, 0), u0 + uT);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering(0, 1), u0 + uD);
    EXPECT_FLOAT_EQ(nondefaultMedium->scattering(1, 1), u0 + uT + uD);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy(0, 0), g0);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy(1, 0), g0 + gT);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy(0, 1), g0 + gD);
    EXPECT_FLOAT_EQ(nondefaultMedium->anisotropy(1, 1), g0 + gT + gD);
}

TEST_F(MediumLinearTests, ConstructorFromMediumProperties) {
    MediumProperties<float> properties;
    properties.n0 = n0;
    properties.nT = nT;
    properties.nD = nD;
    properties.a0 = a0;
    properties.aT = aT;
    properties.aD = aD;
    properties.u0 = u0;
    properties.uT = uT;
    properties.uD = uD;
    properties.g0 = g0;
    properties.gT = gT;
    properties.gD = gD;
    auto generatedMedium = std::make_unique<MediumLinear<float>>(properties);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(0, 0), n0);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(1, 0), n0 + nT);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(0, 1), n0 + nD);
    EXPECT_FLOAT_EQ(generatedMedium->refraction(1, 1), n0 + nT + nD);
    EXPECT_FLOAT_EQ(generatedMedium->absorption(0, 0), a0);
    EXPECT_FLOAT_EQ(generatedMedium->absorption(1, 0), a0 + aT);
    EXPECT_FLOAT_EQ(generatedMedium->absorption(0, 1), a0 + aD);
    EXPECT_FLOAT_EQ(generatedMedium->absorption(1, 1), a0 + aT + aD);
    EXPECT_FLOAT_EQ(generatedMedium->scattering(0, 0), u0);
    EXPECT_FLOAT_EQ(generatedMedium->scattering(1, 0), u0 + uT);
    EXPECT_FLOAT_EQ(generatedMedium->scattering(0, 1), u0 + uD);
    EXPECT_FLOAT_EQ(generatedMedium->scattering(1, 1), u0 + uT + uD);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy(0, 0), g0);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy(1, 0), g0 + gT);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy(0, 1), g0 + gD);
    EXPECT_FLOAT_EQ(generatedMedium->anisotropy(1, 1), g0 + gT + gD);
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
