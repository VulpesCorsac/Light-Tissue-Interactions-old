#pragma once

#ifndef ASSERT_INPUT_PARAMS
    #define ASSERT_INPUT_PARAMS
#endif // ASSERT_INPUT_PARAMS

#include "MediumFactory.h"

#include <gtest/gtest.h>

using namespace Physics_NS;

TEST(MediumFactoryTests, ThrowExceptionIfUnknownMediumType) {
    auto properties = MediumProperties<float>();
    properties.type = MediumType::Unknown;
    EXPECT_THROW(auto medium = createMedium<float>(properties), std::invalid_argument);
}

TEST(MediumFactoryTests, CreateGlassMediumType) {
    auto properties = MediumProperties<float>();
    properties.type = MediumType::Glass;
    auto medium = createMedium<float>(properties);
    EXPECT_EQ(medium->type, properties.type);
}

TEST(MediumFactoryTests, CreateConstantMediumType) {
    auto properties = MediumProperties<float>();
    properties.type = MediumType::Constant;
    auto medium = createMedium<float>(properties);
    EXPECT_EQ(medium->type, properties.type);
}

TEST(MediumFactoryTests, CreateLinearMediumType) {
    auto properties = MediumProperties<float>();
    properties.type = MediumType::Linear;
    auto medium = createMedium<float>(properties);
    EXPECT_EQ(medium->type, properties.type);
}

TEST(MediumFactoryTests, CreateArbitraryMediumType) {
    auto properties = MediumProperties<float>();
    properties.type = MediumType::Arbitrary;
    auto medium = createMedium<float>(properties);
    EXPECT_EQ(medium->type, properties.type);
}
