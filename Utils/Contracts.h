#pragma once

#include <stdexcept>
#include <string>

namespace Utils_NS {
    template < typename Exception >
    class Contract {
    public:
        static void Except(const std::string& file, int line, const std::string& contract, bool value);
    };
}

#ifdef ENABLE_CHECK_CONTRACTS
    #define CHECK_CONTRACT(CONTRACT,EXCEPTION)                        \
        Utils_NS::Contract<EXCEPTION>::Except(std::string(__FILE__),  \
                                              __LINE__,               \
                                              std::string(#CONTRACT), \
                                              CONTRACT);
    #define EXCEPT_INPUT_PARAMS
#else
    #define CHECK_CONTRACT(CONTRACT,EXCEPTION)
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
