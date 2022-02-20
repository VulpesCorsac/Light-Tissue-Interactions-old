#pragma once

#include <stdexcept>
#include <string>

namespace Utils_NS {
    template < typename Exception >
    class Contract {
    public:
        static void Except(const std::string& file, int line, const std::string& contract, bool value);
        static void Fail(const std::string& file, int line, const std::string& reason);
    };
}

#define CHECK_ARGUMENT_CONTRACT(CONTRACT) CHECK_CONTRACT(CONTRACT, std::invalid_argument)
#define CHECK_RUNTIME_CONTRACT(CONTRACT) CHECK_CONTRACT(CONTRACT, std::runtime_error)
#define CHECK_RANGE_CONTRACT(CONTRACT) CHECK_CONTRACT(CONTRACT, std::out_of_range)

#define FAIL_ARGUMENT_CONTRACT(REASON) FAIL_CONTRACT(REASON, std::invalid_argument)
#define FAIL_RUNTIME_CONTRACT(REASON) FAIL_CONTRACT(REASON, std::runtime_error)
#define FAIL_RANGE_CONTRACT(REASON) FAIL_CONTRACT(REASON, std::out_of_range)

#ifdef ENABLE_CHECK_CONTRACTS
    #define CHECK_CONTRACT(CONTRACT,EXCEPTION) Utils_NS::Contract<EXCEPTION>::Except(std::string(__FILE__), __LINE__, std::string(#CONTRACT),  CONTRACT)
    #define FAIL_CONTRACT(REASON,EXCEPTION) throw EXCEPTION(std::string(__FILE__) + ":" + std::to_string(__LINE__) + " - " + std::string(#REASON))
    #define EXCEPT_INPUT_PARAMS
#else
    #define CHECK_CONTRACT(CONTRACT,EXCEPTION)
    #define FAIL_CONTRACT(REASON,EXCEPTION)
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ENABLE_CHECK_CONTRACTS

/******************
 * IMPLEMENTATION *
 ******************/

template < typename Exception >
void Utils_NS::Contract<Exception>::Except(const std::string& file, int line, const std::string& contract, bool value) {
    if (!(value))
        throw Exception(file + ":" + std::to_string(line) + " - " + contract + " failed");
}

template < typename Exception >
void Utils_NS::Contract<Exception>::Fail(const std::string& file, int line, const std::string& reason) {
    throw Exception(file + ":" + std::to_string(line) + " - " + reason + " failed");
}
