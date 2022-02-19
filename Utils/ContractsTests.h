#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "Contracts.h"

#include <gtest/gtest.h>

using namespace Utils_NS;

TEST(ContractsTests, ContractViolation) {
    EXPECT_THROW(CHECK_CONTRACT(false, std::logic_error), std::logic_error);
}

TEST(ContractsTests, NoContractViolation) {
    EXPECT_NO_THROW(CHECK_CONTRACT(true, std::logic_error));
}

TEST(ContractsTests, ArgumentContractViolation) {
    EXPECT_THROW(CHECK_ARGUMENT_CONTRACT(false), std::invalid_argument);
}

TEST(ContractsTests, NoArgumentContractViolation) {
    EXPECT_NO_THROW(CHECK_ARGUMENT_CONTRACT(true));
}

TEST(ContractsTests, RuntimeContractViolation) {
    EXPECT_THROW(CHECK_RUNTIME_CONTRACT(false), std::runtime_error);
}

TEST(ContractsTests, NoRuntimeContractViolation) {
    EXPECT_NO_THROW(CHECK_RUNTIME_CONTRACT(true));
}
