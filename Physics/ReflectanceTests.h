#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "Reflectance.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

TEST(ReflectanceTests, FresnelReflectance_ThrowsExceptionIncidenceRefractionIndex) {
    constexpr float ni  = 0;
    constexpr float nt  = 1;
    constexpr float cos = 0;
    EXPECT_THROW(FresnelReflectance(ni, nt, cos), std::invalid_argument);
}

TEST(ReflectanceTests, FresnelReflectance_ThrowsExceptionTransmittanceRefractionIndex) {
    constexpr float ni  = 1;
    constexpr float nt  = 0;
    constexpr float cos = 0;
    EXPECT_THROW(FresnelReflectance(ni, nt, cos), std::invalid_argument);
}

TEST(ReflectanceTests, FresnelReflectance_ThrowsExceptionTransmittanceIncidenceCosLarge) {
    constexpr float ni  = 1;
    constexpr float nt  = 1;
    constexpr float cos = 2;
    EXPECT_THROW(FresnelReflectance(ni, nt, cos), std::invalid_argument);
}

TEST(ReflectanceTests, FresnelReflectance_ThrowsExceptionTransmittanceIncidenceCosSmall) {
    constexpr float ni  = 1;
    constexpr float nt  = 1;
    constexpr float cos = -2;
    EXPECT_THROW(FresnelReflectance(ni, nt, cos), std::invalid_argument);
}

TEST(ReflectanceTests, BorderReflectance_ThrowsExceptionMediumRefractionIndex) {
    constexpr float nm = 0;
    constexpr float no = 1;
    EXPECT_THROW(BorderReflectance(nm, no), std::invalid_argument);
}

TEST(ReflectanceTests, BorderReflectance_ThrowsExceptionOuterRefractionIndex) {
    constexpr float nm = 1;
    constexpr float no = 0;
    EXPECT_THROW(BorderReflectance(nm, no), std::invalid_argument);
}

/// TODO: add normal tests with some examples
