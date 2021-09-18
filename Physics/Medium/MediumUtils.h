#pragma once

#include "MediumInterface.h"
#include "MediumGlass.h"
#include "MediumType.h"

#include <string>

#ifdef ASSERT_INPUT_PARAMS
    #include <stdexcept>
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

namespace Physics_NS {
    /// Medium type getter from MediumInterface pointer
    /// \param[in] medium MediumInterface pointer
    /// \return MediumType, MediumType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument If ASSERT_INPUT_PARAMS is defined and cannot convert to the known types
    template < typename T >
    MediumType mediumType(MediumInterface<T>* medium) EXCEPT_INPUT_PARAMS;

    /// Medium type getter from MediumInterface pointer
    /// \param[in] medium Medium name
    /// \return MediumType, MediumType::Unknown if cannot convert to the known types
    /// \throw std::invalid_argument If ASSERT_INPUT_PARAMS is defined and cannot convert to the known types
    MediumType mediumType(const std::string& medium) EXCEPT_INPUT_PARAMS;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumType Physics_NS::mediumType(Physics_NS::MediumInterface<T>* medium) EXCEPT_INPUT_PARAMS {
    if (dynamic_cast<Physics_NS::MediumGlass<T>*>(medium))
        return Physics_NS::MediumType::Glass;

    #ifdef ASSERT_INPUT_PARAMS
        throw std::invalid_argument("Medium type cannot be evaluated");
    #endif // ASSERT_INPUT_PARAMS

    return Physics_NS::MediumType::Unknown;
}

Physics_NS::MediumType Physics_NS::mediumType(const std::string& medium) EXCEPT_INPUT_PARAMS {
    auto lower = medium;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); } );

    if (lower == "glass")
        return Physics_NS::MediumType::Glass;

    #ifdef ASSERT_INPUT_PARAMS
        throw std::invalid_argument("Medium type cannot be evaluated");
    #endif // ASSERT_INPUT_PARAMS

    return Physics_NS::MediumType::Unknown;
}
