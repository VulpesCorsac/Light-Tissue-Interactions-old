#pragma once

#include "MediumConstant.h"
#include "MediumInterface.h"
#include "MediumGlass.h"
#include "MediumLinear.h"
#include "MediumProperties.h"
#include "MediumType.h"

#include <memory>

#ifdef ASSERT_INPUT_PARAMS
    #include <stdexcept>
    #define EXCEPT_INPUT_PARAMS
#else
    #define EXCEPT_INPUT_PARAMS noexcept
#endif // ASSERT_INPUT_PARAMS

namespace Physics_NS {
    /// \brief factory function to create medium of needed type based on properties
    /// \param[in] properties glass MediumProperties
    /// \return medium of needed type based on properties
    /// \throw std::invalid_argument If ASSERT_INPUT_PARAMS is defined and properties.type is Unknown
    template < typename T >
    std::unique_ptr<MediumInterface<T>> createMedium(const Physics_NS::MediumProperties<T>& properties) EXCEPT_INPUT_PARAMS;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
std::unique_ptr<MediumInterface<T>> Physics_NS::createMedium(const Physics_NS::MediumProperties<T>& properties) EXCEPT_INPUT_PARAMS {
    switch (properties.type) {
        case Physics_NS::MediumType::Glass:
            return std::unique_ptr<Physics_NS::MediumInterface<T>>(new Physics_NS::MediumGlass<T>(properties));
        case Physics_NS::MediumType::Constant:
            return std::unique_ptr<Physics_NS::MediumInterface<T>>(new Physics_NS::MediumConstant<T>(properties));
        case Physics_NS::MediumType::Linear:
            return std::unique_ptr<Physics_NS::MediumInterface<T>>(new Physics_NS::MediumLinear<T>(properties));
        case Physics_NS::MediumType::Arbitrary:
            return std::unique_ptr<Physics_NS::MediumInterface<T>>(new Physics_NS::MediumArbitrary<T>(properties));
        default:
            #ifdef ASSERT_INPUT_PARAMS
                throw std::invalid_argument("Invalid tissue type in properties");
            #endif // ASSERT_INPUT_PARAMS
            return nullptr;
    }
}

