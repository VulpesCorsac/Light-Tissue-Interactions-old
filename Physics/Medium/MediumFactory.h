#pragma once

#include "MediumArbitrary.h"
#include "MediumConstant.h"
#include "MediumGlass.h"
#include "MediumInterface.h"
#include "MediumLinear.h"
#include "MediumProperties.h"
#include "MediumType.h"

#include <memory>

#include "../../Utils/Contracts.h"

namespace Physics_NS {
    /// \brief factory function to create medium of needed type based on properties
    /// \param[in] properties MediumProperties
    /// \return medium of needed type based on properties
    /// \throw std::invalid_argument if ENABLE_CHECK_CONTRACTS is defined and properties.type is Unknown
    template < typename T >
    std::unique_ptr<MediumInterface<T>> createMedium(const MediumProperties<T>& properties) EXCEPT_INPUT_PARAMS;
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
std::unique_ptr<Physics_NS::MediumInterface<T>> Physics_NS::createMedium(const Physics_NS::MediumProperties<T>& properties) EXCEPT_INPUT_PARAMS {
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
            FAIL_ARGUMENT_CONTRACT("Invalid tissue type in properties");
            return nullptr;
    }
}
