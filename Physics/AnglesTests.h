#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "Angles.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

TEST(AnglesTests, TransmittanceCos_ThrowsExceptionIncidenceRefractionIndex) {
    constexpr float ni  = 0;
    constexpr float nt  = 1;
    constexpr float cos = 0;
    EXPECT_THROW(TransmittanceCos(ni, nt, cos), std::invalid_argument);
}

TEST(AnglesTests, TransmittanceCos_ThrowsExceptionTransmittanceRefractionIndex) {
    constexpr float ni  = 1;
    constexpr float nt  = 0;
    constexpr float cos = 0;
    EXPECT_THROW(TransmittanceCos(ni, nt, cos), std::invalid_argument);
}

TEST(AnglesTests, TransmittanceCos_ThrowsExceptionTransmittanceIncidenceCosLarge) {
    constexpr float ni  = 1;
    constexpr float nt  = 1;
    constexpr float cos = 2;
    EXPECT_THROW(TransmittanceCos(ni, nt, cos), std::invalid_argument);
}

TEST(AnglesTests, TransmittanceCos_ThrowsExceptionTransmittanceIncidenceCosSmall) {
    constexpr float ni  =  1;
    constexpr float nt  =  1;
    constexpr float cos = -2;
    EXPECT_THROW(TransmittanceCos(ni, nt, cos), std::invalid_argument);
}

TEST(AnglesTests, CriticalCos_ThrowsExceptionIncidenceRefractionIndex) {
    constexpr float ni = 0;
    constexpr float nt = 1;
    EXPECT_THROW(CriticalCos(ni, nt), std::invalid_argument);
}

TEST(AnglesTests, CriticalCos_ThrowsExceptionTransmittanceRefractionIndex) {
    constexpr float ni = 1;
    constexpr float nt = 0;
    EXPECT_THROW(CriticalCos(ni, nt), std::invalid_argument);
}

/// TODO: add normal tests with some examples
