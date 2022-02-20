#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "BugerLambert.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

TEST(BugerLambert, ThrowExceptionMediumRefractionIndex) {
    constexpr float nm = 0;
    constexpr float nt = 1;
    constexpr float nb = 1;
    constexpr float a  = 1;
    EXPECT_THROW(BugerLambert(a, nm, nt, nb), std::invalid_argument);
}

TEST(BugerLambert, ThrowExceptionOuterRefractionIndexTop) {
    constexpr float nm = 1;
    constexpr float nt = 0;
    constexpr float nb = 1;
    constexpr float a  = 1;
    EXPECT_THROW(BugerLambert(a, nm, nt, nb), std::invalid_argument);
}

TEST(BugerLambert, ThrowExceptionOuterRefractionIndexBottom) {
    constexpr float nm = 1;
    constexpr float nt = 1;
    constexpr float nb = 0;
    constexpr float a  = 1;
    EXPECT_THROW(BugerLambert(a, nm, nt, nb), std::invalid_argument);
}

TEST(BugerLambert, WorksWithPositiveAbsorption) {
    constexpr float nm = 1;
    constexpr float nt = 1;
    constexpr float nb = 1;
    constexpr float a  = 1;
    EXPECT_NO_THROW(BugerLambert(a, nm, nt, nb));
}

TEST(BugerLambert, WorksWithZeroAbsorption) {
    constexpr float nm = 1;
    constexpr float nt = 1;
    constexpr float nb = 1;
    constexpr float a  = 0;
    EXPECT_NO_THROW(BugerLambert(a, nm, nt, nb));
}

TEST(BugerLambert, WorksWithNegativeAbsorption) {
    constexpr float nm =  1;
    constexpr float nt =  1;
    constexpr float nb =  1;
    constexpr float a  = -1;
    EXPECT_NO_THROW(BugerLambert(a, nm, nt, nb));
}

/// TODO: add normal tests with some examples
