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
    using namespace std;

    switch (properties.type) {
        case MediumType::Glass:
            return unique_ptr<MediumInterface<T>>(new MediumGlass<T>(properties));
        case MediumType::Constant:
            return unique_ptr<MediumInterface<T>>(new MediumConstant<T>(properties));
        case MediumType::Linear:
            return unique_ptr<MediumInterface<T>>(new MediumLinear<T>(properties));
        case MediumType::Arbitrary:
            return unique_ptr<MediumInterface<T>>(new MediumArbitrary<T>(properties));
        default:
            FAIL_ARGUMENT_CONTRACT("Invalid tissue type in properties");
            return nullptr;
    }
}
