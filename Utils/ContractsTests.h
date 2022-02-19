#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "Contracts.h"

#include <gtest/gtest.h>

using namespace Utils_NS;

TEST(ContractsTests, ContractViolation) {
    EXPECT_THROW(CHECK_CONTRACT(false, std::invalid_argument), std::invalid_argument);
}

TEST(ContractsTests, NoContractViolation) {
    EXPECT_NO_THROW(CHECK_CONTRACT(true, std::invalid_argument));
}
